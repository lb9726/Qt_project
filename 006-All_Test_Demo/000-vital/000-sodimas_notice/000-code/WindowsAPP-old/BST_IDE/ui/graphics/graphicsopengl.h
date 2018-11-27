#ifndef GRAPHICSOPENGL_H
#define GRAPHICSOPENGL_H

#include "graphicsoperate.h"

#ifdef SURRPORT_3D
#include "glut.h"

#include "lib3ds/file.h"
#include "lib3ds/camera.h"
#include "lib3ds/mesh.h"
#include "lib3ds/node.h"
#include "lib3ds/material.h"
#include "lib3ds/matrix.h"
#include "lib3ds/vector.h"
#include "lib3ds/light.h"
#endif

#define	CBX	0.25	// camera body dimensions
#define	CBY	1.5
#define	CBZ	1.0

// texture size: by now minimum standard
#define	TEX_XSIZE	1024
#define	TEX_YSIZE	1024

#define	NA(a)	(sizeof(a)/sizeof(a[0]))

#ifndef	MIN
#define	MIN(a,b) ((a)<(b)?(a):(b))
#define	MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifdef SURRPORT_3D
struct _player_texture
{
    int valid; // was the loading attempt successful ?
#ifdef	USE_SDL
    SDL_Surface *bitmap;
#else
    void *bitmap;
#endif
    GLuint tex_id; //OpenGL texture ID
    float scale_x, scale_y; // scale the texcoords, as OpenGL thinks in TEX_XSIZE and TEX_YSIZE
};

typedef struct _player_texture Player_texture;
#endif

typedef	enum {ROTATING, WALKING} RunMode;

#ifdef IDE
    #define ComOpenGL  DesignOpenGL
#elif DISPLAY
    #define ComOpenGL  GraphicsOpenGL
#endif

class GraphicsOpenGL : public GraphicsOperate
{
    Q_OBJECT
public:
    GraphicsOpenGL(QGraphicsItem *parent);
    ~GraphicsOpenGL()
    {
        Unload3DS();
    }
    bool Start();
    virtual QSize GetRcSize(QString pRcName) { return QSize(); }
    virtual QList<QAction*> GetActionList(){ QList<QAction*> tmpList; return tmpList; }

public slots:
    virtual void ExecAction(QAction *pAction) {}
    void updateEffect(AREA_OPERATE pOperate, QVariant pPara);
    void UpdateCom(int pFrame) { GraphicsComponent::UpdateCom(pFrame); }

public:
    void updateEffect(int pState);

public:
    void InitOpenGL();
    void solidBox(double bx, double by, double bz);
    void solidCylinder(double r, double h, int slices);
    void CreateCameraAndLight();
#ifdef SURRPORT_3D
    void RenderNode(Lib3dsNode *node);
    void UpdateLight(Lib3dsLight *l);
    void DrawBounds(Lib3dsVector tgt);
    void DrawLight(const GLfloat *pos, const GLfloat *color);
#endif
    void Display();

    void Resize(int w, int h);

    void Unload3DS();
    bool Load3DS(QString pFileName);
    void Init3DSPara()
    {
#ifdef SURRPORT_3D
        m_model = 0;
#endif
        runMode = ROTATING;
        filepath=0;
        dbuf=1;
        halt=0;
        flush=0;
        anti_alias=1;

        camera=0;
        current_frame=0.0;
        gl_width = 0;
        gl_height  = 0;
        menu_id=0;
        show_object=1;
        show_bounds=0;
        rotating = 0;
        show_cameras = 0;
        show_lights = 0;

        view_rotx = view_roty = view_rotz = anim_rotz = 0.0;
    }

public:
    int                 m_CurRcLabel;

    RunMode runMode;

    char* filepath;
    char datapath[256];
    char filename[256];
    int dbuf;
    int halt;
    int flush;
    int anti_alias;

    char* camera;
    float current_frame;
    int gl_width;
    int gl_height;
    int menu_id;
    int show_object;
    int show_bounds;
    int rotating;
    int show_cameras;
    int show_lights;

    int cameraList, lightList;	/* Icon display lists */

#ifdef SURRPORT_3D
    Lib3dsFile         *m_model;
    Lib3dsVector bmin, bmax;
    Player_texture *pt;
    int tex_mode; // Texturing active ?
#endif

    float	sx, sy, sz, size;	/* bounding box dimensions */
    float	cx, cy, cz;		/* bounding box center */

    float	view_rotx, view_roty, view_rotz;
    float	anim_rotz;

    int	mx, my;
};

class DesignOpenGL : public GraphicsOpenGL
{
    Q_OBJECT
public:
    DesignOpenGL(QGraphicsItem *parent);

    //QSize GetRcSize(QString pRcName){ return size().toSize(); }
    QList<QAction*> GetActionList();

    bool PaintEffect(QPainter *p)
    {
        GraphicsOpenGL::PaintEffect(p);
        //>@»æÖÆ±ß¿ò
        QRectF tmpRect = geometry();
        qreal Width = tmpRect.width();
        qreal Height = tmpRect.height();
        p->translate(Width / 2, Height / 2);
        p->setPen(Qt::DashDotDotLine);
        p->setPen(QColor(10,10,255));
        p->drawRect(QRect(-Width/2, -Height/2, Width, Height));
        p->drawRect(QRect(-Width/2+1, -Height/2+1, Width-2, Height-2));
        return true;
    }

public slots:
    void ExecAction(QAction *pAction);
};

#endif // GRAPHICSQML_H
