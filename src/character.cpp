/*  Open Manifold source file
*   
*   This program/source code is licensed under the MIT License:
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
*/

#include <cstdlib>

#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

using nlohmann::json;

struct character_frames {
    std::vector<SDL_Rect> idle;
    SDL_Rect up;
    SDL_Rect down;
    SDL_Rect left;
    SDL_Rect right;
    SDL_Rect circle;
    SDL_Rect square;
    SDL_Rect triangle;
    SDL_Rect xplode;
    SDL_Rect scale_up;
    SDL_Rect scale_down;
};

enum character_states {
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CIRCLE,
    SQUARE,
    TRIANGLE,
    XPLODE,
    SCALE_UP,
    SCALE_DOWN
};

character_states current_state = IDLE;

character_frames frames = {
    {{0, 0, 0, 0}},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

void set_character_status(char opcode) {
    // convert opcode to state
    switch (opcode) {
        case 'U': current_state = UP; break;
        case 'D': current_state = DOWN; break;
        case 'L': current_state = LEFT; break;
        case 'R': current_state = RIGHT; break;
        case 'Z': current_state = CIRCLE; break;
        case 'X': current_state = SQUARE; break;
        case 'C': current_state = TRIANGLE; break;
        case 'V': current_state = XPLODE; break;
        case 'A': current_state = SCALE_DOWN; break;
        case 'S': current_state = SCALE_UP; break;
        case '.': current_state = IDLE; break;
        default: break;
    }
    return;
}

void reset_character_status() {
    current_state = IDLE;
    return;
}

void parse_character_file(json file) {
    // loads a JSON character file and puts its image coordinate data into frames
    // ----------------------------------------------------------
    // file: a JSON character object

    character_frames frame_data = {
        {{0, 0, 0, 0}},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    // write the idle frame array
    std::vector<SDL_Rect> temp_vector;

    for (int i = 0; i < file[0]["frames"].size(); i++) {
        SDL_Rect temp_rect;

        temp_rect.x = file[0]["frames"][i].value("x", 0);
        temp_rect.y = file[0]["frames"][i].value("y", 0);
        temp_rect.w = file[0]["frames"][i].value("w", 0);
        temp_rect.h = file[0]["frames"][i].value("h", 0);

        temp_vector.push_back(temp_rect);
    }

    frame_data.idle = temp_vector;

    // write the other SDL_Rects; these are relatively simple as they're always going to have 1 entry total
    for (int i = 1; i < 11; i++) {
        SDL_Rect temp_rect;

        temp_rect.x = file[i].value("x", 0);
        temp_rect.y = file[i].value("y", 0);
        temp_rect.w = file[i].value("w", 0);
        temp_rect.h = file[i].value("h", 0);

        switch (i) {
            case 1:  frame_data.up          = temp_rect; break;
            case 2:  frame_data.left        = temp_rect; break;
            case 3:  frame_data.down        = temp_rect; break;
            case 4:  frame_data.right       = temp_rect; break;
            case 5:  frame_data.circle      = temp_rect; break;
            case 6:  frame_data.square      = temp_rect; break;
            case 7:  frame_data.triangle    = temp_rect; break;
            case 8:  frame_data.xplode      = temp_rect; break;
            case 9:  frame_data.scale_up    = temp_rect; break;
            case 10: frame_data.scale_down  = temp_rect; break;
            default: break;
        }
    }

    frames = frame_data;
    return;
}

SDL_Rect get_character_rect(int beat_count) {
    // returns the SDL_Rect for the character's tile map
    // ----------------------------------------------------------
    // beat_count: the current beat count, used for cycling idle frames

    switch (current_state) {
        case IDLE: return frames.idle[beat_count % frames.idle.size()]; break;
        case UP: return frames.up; break;
        case DOWN: return frames.down; break;
        case LEFT: return frames.left; break;
        case RIGHT: return frames.right; break;
        case CIRCLE: return frames.circle; break;
        case SQUARE: return frames.square; break;
        case TRIANGLE: return frames.triangle; break;
        case XPLODE: return frames.xplode; break;
        case SCALE_UP: return frames.scale_up; break;
        case SCALE_DOWN: return frames.scale_down; break;
    }

    return {0, 0, 0, 0};
}