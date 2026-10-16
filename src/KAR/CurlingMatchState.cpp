#include "Curling.hpp"
#include "CurlingTypes.hpp"

#include "../Camera.hpp"
#include "../GameObjects.hpp"
#include "../Input.hpp"
#include "../Map.hpp"
#include "../Model.hpp"
#include "../Sound.hpp"
#include "../extern/KAR.hpp"
#include "../extern/dw1.hpp"
#include "../extern/libc.hpp"
#include "../extern/libgte.hpp"

bool KAR_moveMatchCamera(int32_t x, int32_t z, int32_t speed)
{
    const bool done               = tickCameraMoveTo(static_cast<int16_t>(x), static_cast<int16_t>(z), speed);
    const int32_t cameraRemainder = CAMERA_Y % 128;
    if (cameraRemainder == 0 || cameraRemainder >= 106) {
        const int32_t mapWidth = static_cast<int8_t>(MAP_WIDTH);
        const int32_t tileY    = static_cast<int8_t>(MAP_TILE_Y);
        const int32_t tileX    = static_cast<int8_t>(MAP_TILE_X);
        uploadMapTileImages(MAP_TILE_DATA.data(), tileX + tileY * mapWidth);
    }
    return done;
}

namespace
{
    constexpr int32_t RESULT_PROGRESS_STAT = 0x79;
    constexpr auto HORIZONTAL_INPUT = static_cast<InputButtons>(InputButtons::BUTTON_LEFT | InputButtons::BUTTON_RIGHT);

    Vector cameraStonePosition{};
    uint32_t throwSoundMask = 0;

    void finishMatch()
    {
        // Repeated calls after the match finishes must not repeat teardown.
        if (KAR_MATCH_STATE == CurlingMatchState::FINISHED) {
            return;
        }

        KAR_MATCH_STATE                                                      = CurlingMatchState::FINISHED;
        ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->rotation.y = 0x800;
        ENTITY_TABLE.getEntityById(1)->posData->rotation.y                   = 0x800;
        ENTITY_TABLE.getEntityById(0)->posData->rotation.y                   = 0x800;
        startAnimation(ENTITY_TABLE.getEntityById(0), 2);
        startAnimation(ENTITY_TABLE.getEntityById(1), 2);
        startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 2);
        KAR_resetHints();
        SOME_SCRIPT_SYNC_BIT = 1;
        removeObject(ObjectID::KAR, KAR_OBJECT_INSTANCE);
        setTextColor(1);

        if (KAR_STONE_ROWS[0].score > KAR_STONE_ROWS[1].score) {
            if (KAR_STONE_ROWS[0].score >= 0xa) {
                writePStat(RESULT_PROGRESS_STAT, 2);
            }
            else {
                writePStat(RESULT_PROGRESS_STAT, 1);
            }
        }
        else {
            writePStat(RESULT_PROGRESS_STAT, 0);
        }
    }

    [[gnu::optimize("Os")]]
    void tickMatchState(bool confirmed)
    {
        const int8_t player = KAR_CURRENT_PLAYER != 0 ? static_cast<int8_t>(KAR_OPPONENT_ENTITY) : 0;

        switch (KAR_MATCH_STATE) {
            case CurlingMatchState::INITIALIZE:
                KAR_ANIMATION_TIMER                                                  = 0;
                ENTITY_TABLE.getEntityById(0)->posData->location.x                   = -0x190;
                ENTITY_TABLE.getEntityById(0)->posData->location.z                   = 0x578;
                ENTITY_TABLE.getEntityById(0)->posData->rotation.y                   = 0;
                ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.x = 0x190;
                ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.z = 0x578;
                ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->rotation.y = 0;
                startAnimation(ENTITY_TABLE.getEntityById(0), 0);
                startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0);
                KAR_MATCH_STATE = CurlingMatchState::PROMPT;
                KAR_HINT_STATE  = 0;
                break;
            case CurlingMatchState::PROMPT:
            {
                const auto prompt = KAR_tickYesNoPrompt(confirmed);
                if (prompt == 1) {
                    KAR_MATCH_STATE = CurlingMatchState::READY;
                    KAR_HINT_STATE  = 0;
                }
                else if (prompt == 2) {
                    KAR_MATCH_STATE = CurlingMatchState::REVEAL_DEMO_STONES;
                    KAR_HINT_STATE  = 0;
                    KAR_setupMatch(1);
                }
                break;
            }
            case CurlingMatchState::READY:
                if (KAR_READY_COUNTER >= 0x30) {
                    if ((KAR_ANIMATION_TIMER += 6) >= 0x169) {
                        KAR_ANIMATION_TIMER = 0;
                        KAR_MATCH_STATE     = CurlingMatchState::SELECT_STONE;
                    }
                }
                break;
            case CurlingMatchState::REVEAL_DEMO_STONES:
                if ((KAR_ANIMATION_TIMER += 6) >= 0x169) {
                    KAR_ANIMATION_TIMER = 0;
                    KAR_MATCH_STATE     = CurlingMatchState::SETUP_DEMO_SHOT;
                }
                break;
            case CurlingMatchState::SETUP_DEMO_SHOT:
                KAR_HINT_STATE                                                 = 1;
                KAR_MATCH_STATE                                                = CurlingMatchState::SELECT_STONE;
                KAR_STONE_ROWS[0].plannedStone                                 = static_cast<uint16_t>(rand() % 5);
                KAR_STONE_ROWS[0].shotPower                                    = 0x7D0;
                KAR_STONE_ROWS[0].stones[KAR_STONE_ROWS[0].plannedStone].angle = libgte_ratan2(-0xC1C, 0x190);
                break;
            case CurlingMatchState::SELECT_STONE:
                if ((KAR_CURRENT_PLAYER != 0) || (KAR_HINT_STATE != 0)) {
                    if (KAR_tickHintBox(0, confirmed) != 0) {
                        break;
                    }
                    if (KAR_ANIMATION_TIMER++ >= 0x10) {
                        KAR_ANIMATION_TIMER -= 0xF;
                        if (KAR_SELECTED_STONE == KAR_STONE_ROWS[KAR_CURRENT_PLAYER].plannedStone) {
                            KAR_STONE_ROWS[2].stones[4] = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE];
                            KAR_STONE_ROWS[2].stones[4].state = -0x65;
                            KAR_beginAiming();
                            KAR_ANIMATION_TIMER = 0;
                            if (KAR_HINT_STATE != 0) {
                                KAR_HINT_STATE = 1;
                            }
                        }
                        if (KAR_STONE_ROWS[KAR_CURRENT_PLAYER].plannedStone > KAR_SELECTED_STONE) {
                            KAR_selectNextStone();
                        }
                    }
                }
                else {
                    if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                        KAR_beginAiming();
                    }
                    else if (isKeyDownPolled(InputButtons::BUTTON_UP)) {
                        KAR_selectPreviousStone();
                    }
                    else if (isKeyDownPolled(InputButtons::BUTTON_DOWN)) {
                        KAR_selectNextStone();
                    }
                }
                KAR_AIM_VELOCITY = 0;
                break;
            case CurlingMatchState::AIM:
                if ((KAR_CURRENT_PLAYER != 0) || (KAR_HINT_STATE != 0)) {
                    const int32_t targetAngle = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].angle;
                    if (KAR_tickHintBox(1, confirmed) != 0) {
                        if (isKeyPressed(InputButtons::BUTTON_RIGHT) && (KAR_AIM_ANGLE < -0x18F)) {
                            if (KAR_AIM_VELOCITY > 0) {
                                KAR_AIM_VELOCITY++;
                            }
                            else {
                                KAR_AIM_VELOCITY = 1;
                            }
                            KAR_turnAim(KAR_AIM_VELOCITY);
                        }
                        else if (isKeyPressed(InputButtons::BUTTON_LEFT) && (KAR_AIM_ANGLE >= -0x5B6)) {
                            if (KAR_AIM_VELOCITY < 0) {
                                KAR_AIM_VELOCITY--;
                            }
                            else {
                                KAR_AIM_VELOCITY = -1;
                            }
                            KAR_turnAim(KAR_AIM_VELOCITY);
                        }
                        else if (!isKeyPressed(HORIZONTAL_INPUT)) {
                            KAR_AIM_VELOCITY = 0;
                            break;
                        }
                        else {
                            break;
                        }
                    }
                    if ((KAR_AIM_ANGLE + 0xA >= targetAngle) && (targetAngle >= KAR_AIM_ANGLE - 0xA)) {
                        KAR_beginThrow();
                        if (KAR_HINT_STATE != 0) {
                            KAR_HINT_STATE = 1;
                        }
                    }
                    else if (KAR_AIM_ANGLE < targetAngle)
                        KAR_turnAim(10);
                    else
                        KAR_turnAim(-10);
                }
                else {
                    if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                        KAR_beginThrow();
                    }
                    if (isKeyPressed(InputButtons::BUTTON_RIGHT) && (KAR_AIM_ANGLE < -0x18F)) {
                        if (KAR_AIM_VELOCITY > 0) {
                            KAR_AIM_VELOCITY++;
                        }
                        else {
                            KAR_AIM_VELOCITY = 1;
                        }
                        KAR_turnAim(KAR_AIM_VELOCITY);
                    }
                    if (isKeyPressed(InputButtons::BUTTON_LEFT) && (KAR_AIM_ANGLE >= -0x5B6)) {
                        if (KAR_AIM_VELOCITY < 0) {
                            KAR_AIM_VELOCITY--;
                        }
                        else {
                            KAR_AIM_VELOCITY = -1;
                        }
                        KAR_turnAim(KAR_AIM_VELOCITY);
                    }
                    if (!isKeyPressed(HORIZONTAL_INPUT)) {
                        KAR_AIM_VELOCITY = 0;
                    }
                }
                break;
            case CurlingMatchState::FOCUS_THROW_CAMERA:
                if (KAR_moveMatchCamera(KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].position.x,
                                        KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].position.z,
                                        0x14)) {
                    KAR_MATCH_STATE = CurlingMatchState::SELECT_POWER;
                }
                KAR_AIM_VELOCITY = 0;
                break;
            case CurlingMatchState::SELECT_POWER:
                // The demonstration also uses row 1's threshold and the opponent's rotation below, as in vanilla.
                KAR_THROW_POWER = static_cast<int16_t>(KAR_THROW_POWER + 100);
                if (KAR_THROW_POWER >= 3000) {
                    KAR_MATCH_STATE                                                      = CurlingMatchState::AIM;
                    KAR_THROW_POWER                                                      = 0;
                    ENTITY_TABLE.getEntityById(0)->posData->location.x                   = -0x190;
                    ENTITY_TABLE.getEntityById(0)->posData->location.z                   = 0x578;
                    ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.x = 0x190;
                    ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.z = 0x578;
                    if (KAR_CURRENT_PLAYER == 0) {
                        startAnimation(ENTITY_TABLE.getEntityById(0), 0);
                    }
                    else {
                        startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0);
                    }
                }
                else if ((KAR_CURRENT_PLAYER != 0) || (KAR_HINT_STATE != 0)) {
                    if (KAR_tickHintBox(2, confirmed) != 0) {
                        const int16_t plannedPower = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].shotPower;
                        if (plannedPower <= KAR_THROW_POWER) KAR_THROW_POWER = plannedPower;
                    }
                    else if (KAR_STONE_ROWS[1].shotPower <= KAR_THROW_POWER) {
                        KAR_MATCH_STATE = CurlingMatchState::START_THROW;
                        KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].rotation.y =
                            ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->rotation.y;
                        if (KAR_HINT_STATE != 0) {
                            KAR_HINT_STATE = 1;
                        }
                    }
                }
                else if (isKeyDownPolled(InputButtons::BUTTON_CROSS)) {
                    KAR_MATCH_STATE = CurlingMatchState::START_THROW;
                    KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].angle = KAR_AIM_ANGLE;
                    KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].rotation.y =
                        TAMER_ENTITY.posData->rotation.y;
                }
                break;
            case CurlingMatchState::START_THROW:
                if (KAR_CURRENT_PLAYER == 0) {
                    startAnimation(ENTITY_TABLE.getEntityById(0), 0x25);
                }
                else if (KAR_OPPONENT_ENTITY == 2) {
                    startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0x1E);
                }
                else {
                    startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0x20);
                }
                KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].speed =
                    static_cast<int16_t>(KAR_THROW_POWER + 600);
                KAR_MATCH_STATE     = CurlingMatchState::WAIT_FOR_RELEASE;
                KAR_ANIMATION_TIMER = 0;
                throwSoundMask      = playSound2(8, 0);
                cameraStonePosition = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].position;
                break;
            case CurlingMatchState::WAIT_FOR_RELEASE:
                if (KAR_ANIMATION_TIMER++ >= 3) {
                    KAR_ANIMATION_TIMER                                                 = 0;
                    KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].state = 1;
                    cameraStonePosition = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].position;
                    KAR_MATCH_STATE     = CurlingMatchState::STONES_MOVING;
                }
                break;
            case CurlingMatchState::STONES_MOVING:
            {
                Vector previousPosition = cameraStonePosition;
                cameraStonePosition     = KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[KAR_SELECTED_STONE].position;
                moveCameraByDiff(&previousPosition, &cameraStonePosition);
                break;
            }
            case CurlingMatchState::CLASSIFY_RINGS:
                stopSoundMask(throwSoundMask);
                KAR_classifyStoneRings();
                KAR_MATCH_STATE = CurlingMatchState::TALLY_SCORE;
                for (int32_t rowIndex = 0; rowIndex < 2; rowIndex++) {
                    auto& row  = KAR_STONE_ROWS[rowIndex];
                    row.thrown = 0;
                    for (const auto& stone : row.stones) {
                        if (stone.state != -1) row.thrown++;
                    }
                }
                break;
            case CurlingMatchState::TALLY_SCORE:
                if (KAR_tickScoreTally() != 0) {
                    if (KAR_HINT_STATE != 0) {
                        KAR_MATCH_STATE = CurlingMatchState::FOCUS_DEMO_RESULT;
                    }
                    else {
                        KAR_MATCH_STATE = CurlingMatchState::RESET_TURN;
                    }
                }
                break;
            case CurlingMatchState::FOCUS_DEMO_RESULT:
                if (KAR_moveMatchCamera(0, -0x6A4, 0xA)) {
                    KAR_MATCH_STATE = CurlingMatchState::RESET_TURN;
                }
                break;
            case CurlingMatchState::RESET_TURN:
                if (KAR_tickHintBox(3, confirmed) == 0) {
                    TAMER_ENTITY.posData->location.x                                     = -0x190;
                    TAMER_ENTITY.posData->location.z                                     = 0x578;
                    ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.x = 0x190;
                    ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY)->posData->location.z = 0x578;
                    startAnimation(ENTITY_TABLE.getEntityById(0), 0);
                    startAnimation(ENTITY_TABLE.getEntityById(KAR_OPPONENT_ENTITY), 0);
                    if (KAR_CURRENT_PLAYER == 0) {
                        KAR_PREVIOUS_PLAYER_STONE = KAR_STONE_ROWS[0].stones[KAR_SELECTED_STONE];
                    }
                    if (KAR_HINT_STATE != 0) {
                        if (KAR_moveMatchCamera(ENTITY_TABLE.getEntityById(player)->posData->location.x,
                                                ENTITY_TABLE.getEntityById(player)->posData->location.z,
                                                0x14)) {
                            ENTITY_TABLE.getEntityById(player)->posData->rotation.y = 0;
                            KAR_HINT_STATE                                          = 0;
                            KAR_MATCH_STATE                                         = CurlingMatchState::READY;
                            KAR_setupMatch(0);
                            KAR_ANIMATION_TIMER = 0;
                        }
                    }
                    else {
                        if (KAR_STONE_ROWS[0].thrown == KAR_STONE_ROWS[1].thrown) {
                            if (KAR_STONE_ROWS[0].thrown == 5) {
                                KAR_CURRENT_PLAYER = 1;
                            }
                            else if (KAR_STONE_ROWS[0].score < KAR_STONE_ROWS[1].score) {
                                KAR_CURRENT_PLAYER = 0;
                            }
                            else {
                                KAR_CURRENT_PLAYER = 1;
                            }
                        }
                        else if (KAR_STONE_ROWS[0].thrown < KAR_STONE_ROWS[1].thrown) {
                            KAR_CURRENT_PLAYER = 0;
                        }
                        else {
                            KAR_CURRENT_PLAYER = 1;
                        }
                        for (int32_t stoneIndex = 0; stoneIndex < 5; stoneIndex++) {
                            if (KAR_STONE_ROWS[KAR_CURRENT_PLAYER].stones[stoneIndex].state == -1) {
                                KAR_SELECTED_STONE = static_cast<int16_t>(stoneIndex);
                                break;
                            }
                        }
                        KAR_ANIMATION_TIMER = 0;
                        KAR_THROW_POWER     = 0;
                        KAR_MATCH_STATE     = CurlingMatchState::RETURN_CAMERA;
                    }
                }
                break;
            case CurlingMatchState::RETURN_CAMERA:
                if (KAR_moveMatchCamera(ENTITY_TABLE.getEntityById(player)->posData->location.x,
                                        ENTITY_TABLE.getEntityById(player)->posData->location.z,
                                        0x14)) {
                    if ((KAR_STONE_ROWS[0].thrown >= 5) && (KAR_STONE_ROWS[1].thrown >= 5)) {
                        KAR_HINT_STATE  = 1;
                        KAR_MATCH_STATE = CurlingMatchState::SHOW_HINT;
                        if (KAR_STONE_ROWS[0].score > KAR_STONE_ROWS[1].score) {
                            playSound2(8, 7);
                            KAR_ANIMATION_TIMER = 8;
                        }
                        else {
                            playSound2(8, 8);
                            KAR_ANIMATION_TIMER = 7;
                        }
                    }
                    else if (KAR_CURRENT_PLAYER != 0) {
                        KAR_MATCH_STATE = CurlingMatchState::CHOOSE_OPPONENT_SHOT;
                    }
                    else {
                        KAR_MATCH_STATE = CurlingMatchState::SELECT_STONE;
                    }
                }
                break;
            case CurlingMatchState::CHOOSE_OPPONENT_SHOT:
                KAR_chooseOpponentShot();
                KAR_MATCH_STATE     = CurlingMatchState::SHOW_HINT;
                KAR_HINT_STATE      = 1;
                KAR_ANIMATION_TIMER = 0;
                KAR_THROW_POWER     = 0;
                if (KAR_STONE_ROWS[0].score > KAR_STONE_ROWS[1].score) {
                    KAR_ANIMATION_TIMER = 5;
                }
                else if (KAR_STONE_ROWS[0].score == KAR_STONE_ROWS[1].score) {
                    KAR_ANIMATION_TIMER = 6;
                }
                else {
                    KAR_ANIMATION_TIMER = 4;
                }
                break;
            case CurlingMatchState::WAIT_FOR_HINT_BOX:
                if (KAR_isHintBoxOpen()) {
                    KAR_MATCH_STATE = CurlingMatchState::SHOW_HINT;
                }
                break;
            case CurlingMatchState::SHOW_HINT:
                if (KAR_tickHintBox(KAR_ANIMATION_TIMER, confirmed) == 0) {
                    KAR_HINT_STATE = 0;
                    if (KAR_ANIMATION_TIMER >= 7) {
                        finishMatch();
                    }
                    else {
                        KAR_MATCH_STATE     = CurlingMatchState::SELECT_STONE;
                        KAR_ANIMATION_TIMER = 0;
                        KAR_THROW_POWER     = 0;
                    }
                }
                break;
            case CurlingMatchState::FINISHED:
                KAR_ANIMATION_TIMER = 0;
                KAR_THROW_POWER     = 0;
                finishMatch();
                break;
        }
    }

} // namespace

void KAR_tick()
{
    // Vanilla continues this ordered tail even when the state update finishes the match.
    const bool confirmed = KAR_tickHints();
    tickMatchState(confirmed);
    KAR_renderHints();
    KAR_renderAimArrow();
    KAR_renderStoneCursor();
    KAR_renderScores();
    KAR_renderPowerMeter();
    KAR_renderReadyPrompt();
    KAR_renderNamePlates();
    KAR_handleAimScroll();
    KAR_updateRingMarkers();
    KAR_checkStonesStopped();
}
