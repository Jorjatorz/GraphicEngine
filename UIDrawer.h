#ifndef UIDRAWER_H
#define UIDRAWER_H

#include "Definitions.h"

//From gl_texture_surface awesomium sample - This class will be returned
class UIDrawer : public Awesomium::Surface
{
public:
	virtual void Paint(unsigned char* src_buffer, int src_row_span,	const Awesomium::Rect& src_rect, const Awesomium::Rect& dest_rect) = 0;

	virtual void Scroll(int dx,	int dy,	const Awesomium::Rect& clip_rect) = 0;

	virtual GLuint GetTexture() const = 0;
	virtual int width() const = 0;
	virtual int height() const = 0;
	virtual int size() const = 0;
};

class GLRAMTextureSurface : public UIDrawer {
	GLuint texture_id_;
	unsigned char* buffer_;
	int bpp_, rowspan_, width_, height_;
	bool needs_update_;

public:
	GLRAMTextureSurface(int width, int height);
	virtual ~GLRAMTextureSurface();

	GLuint GetTexture() const;

	int width() const { return width_; }

	int height() const { return height_; }

	int size() const { return rowspan_ * height_; }

protected:
	virtual void Paint(unsigned char* src_buffer, int src_row_span, const Awesomium::Rect& src_rect, const Awesomium::Rect& dest_rect);

	virtual void Scroll(int dx,	int dy,	const Awesomium::Rect& clip_rect);

	void UpdateTexture();
};

//Set as default drawer in awesomiumcore at UIManager
class UITextureSurfaceFactory : public Awesomium::SurfaceFactory {
public:
	UITextureSurfaceFactory();

	virtual ~UITextureSurfaceFactory();

	virtual Awesomium::Surface* CreateSurface(Awesomium::WebView* view,	int width,	int height);

	virtual void DestroySurface(Awesomium::Surface* surface);
};


#endif