bool isLittleEndian() {
    int myInt = 1;
    if (*((char*) &myInt) == 1) {
        return true;
    } else {
        return false;
    }
}

float swapFloatEndian( float x)
{
    union  u {float f; char temp[4];};
    union u un,vn;
    un.f = x;

	if (isLittleEndian()) {
		vn.temp[0] = un.temp[3];
		vn.temp[1] = un.temp[2];
		vn.temp[2] = un.temp[1];
		vn.temp[3] = un.temp[0];
	}

	return vn.f;

}

//use this function to swap the endian-ness of an int.
int swapIntEndian( int x)
{
    union  u {int f; char temp[4];};
    union u un,vn;

	un.f = x;

	if (isLittleEndian()) {
		vn.temp[0] = un.temp[3];
		vn.temp[1] = un.temp[2];
		vn.temp[2] = un.temp[1];
		vn.temp[3] = un.temp[0];
	}
    return vn.f;

}