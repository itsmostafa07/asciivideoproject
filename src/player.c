
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>

#include "player.h"
#include "helpers.h"

static int print_frame(char *filename, WINDOW *frame_pad, int width, int height)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        INFO("Error: Could not open file %s\n", filename);
        return -1;
    }

    werase(frame_pad);
    char line[width + 1];
    for (int y = 0; y < height; y++)
    {
        if (fgets(line, sizeof(line), file) == NULL)
            break;

        mvwprintw(frame_pad, y, 0, "%.*s", width, line);
    }

    fclose(file);
    return 0;
}

video_player *video_player_new(char *src, specs *specs)
{
    video_player *player = malloc(sizeof(video_player));
    player->src = src;
    player->specs = specs;
    return player;
}

int video_player_run(video_player *player)

{

    int delay = 1000000 / player->specs->fps;

    int width = player->specs->width * 4, height = player->specs->height;

    INFO("Video dimensions: width = %i, height = %i\n", width, height);

    int offset_y;
    int offset_x;

    int term_rows, term_cols;
    int prev_term_rows, prev_term_cols;

    getmaxyx(stdscr, prev_term_rows, prev_term_cols);
    WINDOW *frame_pad = newpad(height, width);

    if (!frame_pad)
    {
        ERROR(" Could not create frame pad\n");
        return -1;
    }

    float fps = player->specs->fps;
    uint32_t frames_count = player->specs->frames_count;

    for (int i = 1; i <= player->specs->frames_count; i++)
    {
        werase(frame_pad);
        do
        {
            getmaxyx(stdscr, term_rows, term_cols);

            offset_y = (term_rows - height) / 2;
            offset_x = (term_cols - width) / 2;

            if (offset_y >= 0 && offset_x >= 0)
            {
                break;
            }
            else
            {

                clear();
                mvprintw(term_rows / 2, (term_cols - 30) / 2, "Resize terminal to %dx%d", width, height);
                mvprintw(term_rows / 2 + 1, (term_cols - 30) / 2, "Current: %dx%d", term_cols, term_rows);
                refresh();

                usleep(10000);
            }
        } while (1);

        char filename[128];

        snprintf(filename, sizeof(filename), "%s%i.ascii", player->src, i);

        print_frame(filename, frame_pad, width, height);
        if (term_rows != prev_term_rows || term_cols != prev_term_cols)
        {
            clear();
            refresh();
        }
        prefresh(frame_pad, 0, 0, offset_y, offset_x, offset_y + height - 1, offset_x + width - 1);

        usleep(delay);

        prev_term_rows = term_rows;
        prev_term_cols = term_cols;
    }

    delwin(frame_pad);
    return 0;
}
