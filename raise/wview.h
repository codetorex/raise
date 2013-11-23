#ifndef WVIEW_H
#define WVIEW_H

typedef unsigned char byte;



/**
 * An interface for implementing views with C language.
 */
class WView
{
public:

	virtual void Render() = 0;
};

/*struct WView
{

	void* Model;
	void (*View)();
	void* Controller;


	
};


struct WViewAPI
{
	char* (*UrlToAction)(char* input);
	char* (*UrlToContent)(char* input);
};*/

#endif // !WVIEW_H
