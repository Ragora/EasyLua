/**
 *  @file main.cpp
 *  @brief Main source file for invoking GTest on EasyLua.
 *
 *  This software is licensed under the MIT license. Refer to LICENSE.txt for
 *  more information.
 *
 *  @date 8/10/2016
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Robert MacGregor
 */

#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    return 0;
}
