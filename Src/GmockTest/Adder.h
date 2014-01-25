/* 
 * File:   Adder.h
 * Author: tomko
 *
 * Created on 25 styczeń 2014, 21:09
 */

#ifndef ADDER_H
#define	ADDER_H

class Adder
{
public:
  Adder();
  Adder(const Adder& orig);
  virtual ~Adder();

	int add(int, int);
private:

};

#endif	/* ADDER_H */

