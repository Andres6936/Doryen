class TCODLIB_GUI_API Image : public Widget {
public : 
	Image(int x,int y,int w, int h, const char *tip=NULL);
	virtual ~Image();

    void setBackgroundColor( const Doryen::Color col );
	void render();
protected :
	void expand(int width, int height);

    Doryen::Color back;
};

