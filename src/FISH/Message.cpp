#include "../Font.hpp"
#include "../GameObjects.hpp"
#include "../Helper.hpp"
#include "../Input.hpp"
#include "../Sound.hpp"
#include "../UIBox.hpp"
#include "../UIElements.hpp"
#include "../extern/FISH.hpp"

namespace
{
    constexpr RECT BOUNDS{-130, 42, 262, 59};
    constexpr int32_t DEPTH = 4;

    void tickMessage(int32_t);
    void renderMessage(int32_t);

    class FishingMessage
    {
    public:
        bool isVisible() const { return admitted; }

        void close()
        {
            if (!admitted) return;
            removeObject(ObjectID::FISH_MESSAGE, 0);
            admitted = false;
            rows     = 0;
        }

        void init()
        {
            close();
            frames = 0;
        }

        void show(const uint8_t* line0,
                  const uint8_t* line1,
                  const uint8_t* line2,
                  const uint8_t* line3,
                  const Vector* target)
        {
            close();
            POLLED_INPUT                    = 0;
            FISHING_DATA_PTR->scrollToX     = target->x;
            FISHING_DATA_PTR->scrollToY     = target->y;
            FISHING_DATA_PTR->scrollToZ     = target->z;
            FISHING_DATA_PTR->field53_0x894 = target->pad;
            frames                          = 0;
            TEXTBOX_OPEN_TIMER              = 0;
            // Omit the notice if its callback cannot run; the rod already selected its continuation.
            if (!addObject(ObjectID::FISH_MESSAGE, 0, tickMessage, renderMessage)) return;
            admitted = true;
            frame    = UIBox(BOUNDS, {0, 0, 0}, true);

            // The rod's visibility guard keeps the bait menu out of these four prepared VRAM rows.
            // Consume borrowed names/formatting buffers now, with the inherited font ink unchanged.
            const dtl::array<const uint8_t*, 4> lines{line0, line1, line2, line3};
            libgpu_DrawSync(0);
            clearTextArea();
            for (uint32_t i = 0; i < lines.size(); ++i) {
                if (lines[i] == nullptr) continue;
                rows |= 1 << i;
                libgpu_DrawSync(0);
                drawStringNew(&vanillaFont, lines[i], 704, 256 + i * 12);
                libgpu_DrawSync(0);
            }
        }

        void tick()
        {
            if (!admitted) return;
            FISHING_DATA_PTR->isScrolling = 1;
            if ((frames >= 10 && isKeyDownPolled(InputButtons::BUTTON_CROSS)) ||
                isKeyDownPolled(InputButtons::BUTTON_TRIANGLE)) {
                POLLED_INPUT = 0;
                close();
                playSound(0, 3);
            }
            // Immediate close retires registration, while this owner survives the closing tick's tail.
            if (frames < 100) ++frames;
            ++TEXTBOX_OPEN_TIMER;
        }

        void render()
        {
            if (!admitted) return;
            for (uint32_t i = 0; i < 4; ++i)
                if ((rows & (1 << i)) != 0) renderStringNew(0, -125, 45 + i * 13, 255, 12, 704, 256 + i * 12, DEPTH, 1);
            renderRectPolyFT4(109,
                              85,
                              16,
                              12,
                              TEXTBOX_OPEN_TIMER / 4 % 3 * 16 + 0x4c,
                              0xb7,
                              getTPage(0, 1, 320, 128),
                              getClut(96, 500),
                              DEPTH,
                              0x40);
            frame.render(DEPTH);
        }

    private:
        UIBox frame{};
        int32_t frames{};
        uint8_t rows{};
        bool admitted{};
    };

    // The loader supplies zeroed overlay data; show constructs every frame field before rendering.
    FishingMessage message{};

    void tickMessage(int32_t)
    {
        message.tick();
    }

    void renderMessage(int32_t)
    {
        message.render();
    }
} // namespace

extern "C"
{
    int32_t FISH_isMessageVisible()
    {
        return message.isVisible();
    }

    void FISH_showMessage(const uint8_t* line0,
                          const uint8_t* line1,
                          const uint8_t* line2,
                          const uint8_t* line3,
                          const Vector* target)
    {
        message.show(line0, line1, line2, line3, target);
    }

    void FISH_initMessage()
    {
        message.init();
    }

    void FISH_shutdownMessage()
    {
        message.close();
    }
}
