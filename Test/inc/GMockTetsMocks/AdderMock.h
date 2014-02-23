/* 
 * File:   AdderMock.h
 * Author: tomko
 *
 * Created on 25 styczeń 2014, 21:39
 */

#ifndef ADDERMOCK_H
#define	ADDERMOCK_H

#include <gmock/gmock.h>

#include <Adder.h>

struct AdderMock : public Adder
{
	MOCK_METHOD2(add, int(int, int));
};


#endif	/* ADDERMOCK_H */

