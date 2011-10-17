/*
 * =====================================================================================
 *
 *       Filename:  misc_maths.h
 *
 *    Description:  A few basic maths functions
 *
 *        Version:  1.0
 *        Created:  16/07/11 14:44:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ewan Hemingway, ewan.hemingway@gmail.com
 *
 * =====================================================================================
 */
#include <cstdlib>

inline double randDouble(){
    return ( (double) rand() / (double) RAND_MAX);   // get rand in [0, 1)
}
