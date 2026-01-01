#pragma once

enum class Movies
{
    INTRO,
    NEWGAME,
    POST_CREDITS,
    CREDITS,
};

extern "C"
{
    void MOV_startMovie(Movies movie);
}