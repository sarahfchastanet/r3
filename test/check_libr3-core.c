/*
 * Copyright (C) 2015-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <config.h>

#include <stdlib.h>
#include <time.h>

#include <r3.h>
#include <r3_synclinks.h>

#include "check_libr3.h"

/**
 * @brief
 * Test that we can "test ourselves".
 *
 * @details
 * This test just ensures we can create a passing test. It is a simple, dummy
 * test, for sake of having a test.
 */
START_TEST(test_null)
{
    ck_assert(1);
}
END_TEST

/**
 * @brief
 * Test that we can exercise the basics of the API without crashing anything.
 *
 * @details
 * This test more or less ensures that we don't segfault or otherwise do
 * something silly while making basic API calls.
 */
START_TEST(test_basics)
{
    const int directions[] = {
        R3_UP,
        R3_DOWN,
        R3_LEFT,
        R3_RIGHT,
    };
    r3cube cube;

    // can I init a cube?
    ck_assert_int_eq(0, r3_init(&cube));

    // can I move it in every direction?
    // FIXME: update test to dynamically handle selector max idx
    for (size_t i = 0; i < sizeof(directions)/sizeof(directions[0]); ++i) {
        for (int selector = 0; selector < MAX_ROW_COLS; ++selector) {
            ck_assert_int_eq(0, r3_move(&cube, directions[i], selector));
        }
    }

    // can I manually call synclinks?
    ck_assert_int_eq(0, r3_synclinks(&cube));

    // can I get the facing side?
    ck_assert(NULL != r3_cube_get_face(&cube, 0));

    // can I get a cell?
    ck_assert(NULL != r3_get_cell(&cube, 0, 0, 0));
}
END_TEST

/**
 * @brief
 * Test that we can move in many, many different ways without failing.
 *
 * @details
 * This test issues r3_move calls from a const-seeded prng so as to exercise
 * lots of "arbitrary" calls. The need for this test arose when I was trying to
 * refactor parts of my core algorithm, using the test-suite to ensure I hadn't
 * broken anything: I found that simply issuing r3_move against each direction
 * and selector wasn't thorough enough of a test in some refactoring attempts.
 */
START_TEST(prng_move)
{
    const int directions[] = {
        R3_UP,
        R3_DOWN,
        R3_LEFT,
        R3_RIGHT,
    };
    r3cube cube;

    ck_assert_int_eq(0, r3_init(&cube));

    srand(0);

    for (unsigned i = 0; i < 1024; ++i) {
        unsigned direction = directions[rand() % sizeof(directions)/sizeof(directions[0])];
        unsigned modulus;

        switch (direction) {
            case R3_UP:
            case R3_DOWN:
                modulus = NUM_COLS;
                break;
            case R3_LEFT:
            case R3_RIGHT:
                modulus = NUM_ROWS;
                break;
            default:
                ck_abort_msg("should never reach this case");
        }

        unsigned selector = rand() % modulus;
        ck_assert_int_eq(0, r3_move(&cube, direction, selector));
    }
}
END_TEST

static Suite *builder()
{
    Suite *s;
    TCase *tc;

    s = suite_create("libr3::core");

    tc = tcase_create("null");
    tcase_add_test(tc, test_null);
    suite_add_tcase(s, tc);

    tc = tcase_create("basics");
    tcase_add_test(tc, test_basics);
    tcase_add_test(tc, prng_move);
    suite_add_tcase(s, tc);

    return s;
}
EXPORT_SUITE(builder);

// vim: et ts=4 sw=4 :
