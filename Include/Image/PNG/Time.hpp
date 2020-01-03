#ifndef LIBTCOD_TIME_HPP
#define LIBTCOD_TIME_HPP

/*The information of a Time chunk in PNG.*/
class LodePNGTime
{

public:

	unsigned year;    /*2 bytes used (0-65535)*/
	unsigned month;   /*1-12*/
	unsigned day;     /*1-31*/
	unsigned hour;    /*0-23*/
	unsigned minute;  /*0-59*/
	unsigned second;  /*0-60 (to allow for leap seconds)*/
};

#endif //LIBTCOD_TIME_HPP
