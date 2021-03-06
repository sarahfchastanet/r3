/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <stdlib.h>

#include <r3/r3_get_cell.h>

#include "r3cube.h"
#include "r3sides.h"

r3cell *r3_get_cell(const r3cube *cube, unsigned side, unsigned row, unsigned col)
{
    if (!cube) {
        return NULL;
    }

    if (row >= NUM_ROWS || col >= NUM_COLS || side >= NUM_SIDES) {
        return NULL;
    }

#if !defined(NDEBUG)
    _r3_cube_check_integrity(cube);
#endif

    return cube->sides[side].cells[row][col];
}

// vim: ts=4 sts=4 sw=4 et :
