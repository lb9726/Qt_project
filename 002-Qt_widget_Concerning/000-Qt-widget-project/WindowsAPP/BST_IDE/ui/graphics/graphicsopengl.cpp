#include "graphicsopengl.h"

#ifdef SURRPORT_3D
static const GLfloat white[4] = {1.,1.,1.,1.};
static const GLfloat dgrey[4] = {.25,.25,.25,1.};
static const GLfloat grey[4] = {.5,.5,.5,1.};
static const GLfloat lgrey[4] = {.75,.75,.75,1.};
static const GLfloat black[] = {0.,0.,0.,1.};
static const GLfloat red[4] = {1.,0.,0.,1.};
static const GLfloat green[4] = {0.,1.,0.,1.};
static const GLfloat blue[4] = {0.,0.,1.,1.};
#endif

GraphicsOpenGL::GraphicsOpenGL(QGraphicsItem *parent) :
    GraphicsOperate(parent)
{
    Init3DSPara();
}

bool GraphicsOpenGL::Start()
{
    if(GraphicsOperate::Start() == false)
        return false;
    m_CurRcLabel = -1;
    updateEffect(0); //>@
    return true;
}

void GraphicsOpenGL::updateEffect(AREA_OPERATE pOperate, QVariant pPara)
{
    GraphicsOperate::updateEffect(pOperate, pPara);
    switch(pOperate)
    {
        case OPERATE_CHANGE:
        {
            int index = pPara.toInt();
            //>@如果遇到0xffff，则默认播放下一张
            if(index == NEXTRC)
                index = getNextRc(m_CurRcLabel);
            updateEffect(index);
            break;
        }
        default:
            break;
    }
}

void GraphicsOpenGL::updateEffect(int pState)
{
    bool tmpStateChange = false;
    if(pState != m_CurRcLabel)
        tmpStateChange = true;

    if(tmpStateChange)
    {
        QString tmpFileName = m_EffectRC.value(pState)->mRcFile;
        Load3DS(tmpFileName);
        m_CurRcLabel = pState;
        IDE_TRACE_STR(tmpFileName);
    }
}

void GraphicsOpenGL::Unload3DS()
{
#ifdef SURRPORT_3D
    if(m_model)
    {
        lib3ds_file_free(m_model);
        m_model = 0;
    }
#endif
}

bool GraphicsOpenGL::Load3DS(QString pFileName)
{
#ifdef SURRPORT_3D
    Unload3DS();
    Init3DSPara();
    m_model = lib3ds_file_load(pFileName.toLatin1().data());
    if (!m_model)
    {
        IDE_DEBUG(QString("Unable to load ")+pFileName);
        return false;
    }

    /* No nodes?  Fabricate nodes to display all the meshes. */
    if( !m_model->nodes )
    {
        Lib3dsMesh *mesh;
        Lib3dsNode *node;
        for(mesh = m_model->meshes; mesh ; mesh = mesh->next)
        {
            node = lib3ds_node_new_object();
            strcpy(node->name, mesh->name);
            node->parent_id = LIB3DS_NO_PARENT;
            lib3ds_file_insert_node(m_model, node);
        }
    }

    lib3ds_file_eval(m_model, 1.0f);
    lib3ds_file_bounding_box_of_nodes(m_model, LIB3DS_TRUE, LIB3DS_FALSE, LIB3DS_FALSE, bmin, bmax);
    sx = bmax[0] - bmin[0];
    sy = bmax[1] - bmin[1];
    sz = bmax[2] - bmin[2];
    size = MAX(sx, sy);
    size = MAX(size, sz);
    cx = (bmin[0] + bmax[0])/2;
    cy = (bmin[1] + bmax[1])/2;
    cz = (bmin[2] + bmax[2])/2;

    /* No cameras in the m_model?  Add four */
    if( !m_model->cameras )
    {
        /* Add some cameras that encompass the bounding box */
        Lib3dsCamera *camera = lib3ds_camera_new("Camera_X");
        camera->target[0] = cx;
        camera->target[1] = cy;
        camera->target[2] = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[0] = bmax[0] + 1.5 * MAX(sy,sz);
        camera->near_range = ( camera->position[0] - bmax[0] ) * .5;
        camera->far_range = ( camera->position[0] - bmin[0] ) * 2;
        lib3ds_file_insert_camera(m_model, camera);

        /* Since lib3ds considers +Y to be into the screen, we'll put
        * this camera on the -Y axis, looking in the +Y direction.
        */
        camera = lib3ds_camera_new("Camera_Y");
        camera->target[0] = cx;
        camera->target[1] = cy;
        camera->target[2] = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[1] = bmin[1] - 1.5 * MAX(sx,sz);
        camera->near_range = ( bmin[1] - camera->position[1] ) * .5;
        camera->far_range = ( bmax[1] - camera->position[1] ) * 2;
        lib3ds_file_insert_camera(m_model, camera);

        camera = lib3ds_camera_new("Camera_Z");
        camera->target[0] = cx;
        camera->target[1] = cy;
        camera->target[2] = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[2] = bmax[2] + 1.5 * MAX(sx,sy);
        camera->near_range = ( camera->position[2] - bmax[2] ) * .5;
        camera->far_range = ( camera->position[2] - bmin[2] ) * 2;
        lib3ds_file_insert_camera(m_model, camera);

        camera = lib3ds_camera_new("Camera_ISO");
        camera->target[0] = cx;
        camera->target[1] = cy;
        camera->target[2] = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[0] = bmax[0] + .75 * size;
        camera->position[1] = bmin[1] - .75 * size;
        camera->position[2] = bmax[2] + .75 * size;
        camera->near_range = ( camera->position[0] - bmax[0] ) * .5;
        camera->far_range = ( camera->position[0] - bmin[0] ) * 3;
        lib3ds_file_insert_camera(m_model, camera);
    }

    /* No lights in the m_model?  Add some. */
    if (m_model->lights == 0)
    {
        Lib3dsLight *light;

        light = lib3ds_light_new("light0");
        light->spot_light = 0;
        light->see_cone = 0;
        light->color[0] = light->color[1] = light->color[2] = .6;
        light->position[0] = cx + size * .75;
        light->position[1] = cy - size * 1.;
        light->position[2] = cz + size * 1.5;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier = 1;
        lib3ds_file_insert_light(m_model, light);

        light = lib3ds_light_new("light1");
        light->spot_light = 0;
        light->see_cone = 0;
        light->color[0] = light->color[1] = light->color[2] = .3;
        light->position[0] = cx - size;
        light->position[1] = cy - size;
        light->position[2] = cz + size * .75;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier = 1;
        lib3ds_file_insert_light(m_model, light);

        light = lib3ds_light_new("light2");
        light->spot_light = 0;
        light->see_cone = 0;
        light->color[0] = light->color[1] = light->color[2] = .3;
        light->position[0] = cx;
        light->position[1] = cy + size;
        light->position[2] = cz + size;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier = 1;
        lib3ds_file_insert_light(m_model, light);
    }

    if (!m_model->cameras)
    {
        fputs("3dsplayer: Error: No Camera found.\n", stderr);
        lib3ds_file_free(m_model);
        m_model=0;
        return false;
    }
    if (!camera)
    {
        camera = m_model->cameras->name;
    }

    lib3ds_file_eval(m_model,0.0);
#endif
    return true;
}

/*!
* Initialize OpenGL
*/
void GraphicsOpenGL::InitOpenGL()
{
#ifdef SURRPORT_3D
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    //glDisable(GL_NORMALIZE);
    //glPolygonOffset(1.0, 2);
#endif
}

/*!
* Render a box, centered at 0,0,0
*
* Box may be rendered with face culling enabled.
*/
void GraphicsOpenGL::solidBox(double bx, double by, double bz)
{
#ifdef SURRPORT_3D
    glBegin(GL_POLYGON);
    glNormal3d(0.,0.,1.);
    glVertex3d(bx,by,bz);
    glVertex3d(-bx,by,bz);
    glVertex3d(-bx,-by,bz);
    glVertex3d(bx,-by,bz);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0.,0.,-1.);
    glVertex3d(-bx,by,-bz);
    glVertex3d(bx,by,-bz);
    glVertex3d(bx,-by,-bz);
    glVertex3d(-bx,-by,-bz);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0.,-1.,0.);
    glVertex3d(-bx,by,bz);
    glVertex3d(bx,by,bz);
    glVertex3d(bx,by,-bz);
    glVertex3d(-bx,by,-bz);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(0.,-1.,0.);
    glVertex3d(bx,-by,bz);
    glVertex3d(-bx,-by,bz);
    glVertex3d(-bx,-by,-bz);
    glVertex3d(bx,-by,-bz);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(1.,0.,0.);
    glVertex3d(bx,by,bz);
    glVertex3d(bx,-by,bz);
    glVertex3d(bx,-by,-bz);
    glVertex3d(bx,by,-bz);
    glEnd();
    glBegin(GL_POLYGON);
    glNormal3d(-1.,0.,0.);
    glVertex3d(-bx,by,-bz);
    glVertex3d(-bx,-by,-bz);
    glVertex3d(-bx,-by,bz);
    glVertex3d(-bx,by,bz);
    glEnd();
#endif
}

/*!
* Render a cylinder with end caps, along the Z axis centered at 0,0,0
*
* Cylinder may be rendered with face culling enabled.
*/
void GraphicsOpenGL::solidCylinder(double r, double h, int slices)
{
#ifdef SURRPORT_3D
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    glPushMatrix();
    glTranslated(0., 0., -h/2);
    gluCylinder( qobj, r, r, h, slices, 1 );
    glPushMatrix();
    glRotated(180., 1.,0.,0.);
    gluDisk( qobj, 0., r, slices, 1 );
    glPopMatrix();
    glTranslated(0., 0., h);
    gluDisk( qobj, 0., r, slices, 1 );
    glPopMatrix();
#endif
}

/*!
* Create camera and light icons
*/
void GraphicsOpenGL::CreateCameraAndLight()
{
#ifdef SURRPORT_3D
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    cameraList = glGenLists(1);
    glNewList(cameraList, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dgrey);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lgrey);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glEnable(GL_CULL_FACE);

    //solidBox(CBX,CBY,CBZ);

    glPushMatrix();
    glTranslatef(0.,.9,1.8);
    glRotatef(90., 0.,1.,0.);

    //solidCylinder(1., CBX*2, 12);

    glTranslatef(0.,-1.8,0.);
    solidCylinder(1., CBX*2, 12);
    glPopMatrix();
    glDisable(GL_CULL_FACE);
    glPushMatrix();
    glTranslated(0.,CBY,0.);
    glRotated(-90., 1.,0.,0.);
    gluCylinder(qobj, .2, .5, 1., 12, 1);
    glPopMatrix();
    glEndList();

    lightList = glGenLists(1);
    glNewList(lightList, GL_COMPILE);
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT, dgrey);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dgrey);
    glMaterialfv(GL_FRONT, GL_SPECULAR, grey);
    glEnable(GL_CULL_FACE);
    gluSphere(qobj, .5, 12., 6.);
    glRotated(180.,1.,0.,0.);
    glMaterialfv(GL_FRONT, GL_EMISSION, dgrey);
    gluCylinder(qobj, .2, .2, 1., 12, 1);
    glPopMatrix();
    glEndList();
#endif
}

/*!
* Render node recursively, first children, then parent.
* Each node receives its own OpenGL display list.
*/
#ifdef SURRPORT_3D
void GraphicsOpenGL::RenderNode(Lib3dsNode *node)
{
    Lib3dsNode *p;
    for (p=node->childs; p!=0; p=p->next)
    {
        RenderNode(p);
    }

    if (node->type==LIB3DS_OBJECT_NODE)
    {
        Lib3dsMesh *mesh;

        if (strcmp(node->name,"$$$DUMMY")==0)
            return;

        mesh = lib3ds_file_mesh_by_name(m_model, node->data.object.morph);
        if( !mesh )
            mesh = lib3ds_file_mesh_by_name(m_model, node->name);
        if (!mesh)
            return;

        if (!mesh->user.d)
        {
            mesh->user.d=glGenLists(1);
            glNewList(mesh->user.d, GL_COMPILE);
            {
                unsigned p;
                Lib3dsVector *normalL=(Lib3dsVector *)malloc(3*sizeof(Lib3dsVector)*mesh->faces);
                Lib3dsMaterial *oldmat = (Lib3dsMaterial *)-1;
                {
                    Lib3dsMatrix M;
                    lib3ds_matrix_copy(M, mesh->matrix);
                    lib3ds_matrix_inv(M);
                    glMultMatrixf(&M[0][0]);
                }
                lib3ds_mesh_calculate_normals(mesh, normalL);

                for (p=0; p<mesh->faces; ++p)
                {
                    Lib3dsFace *f=&mesh->faceL[p];
                    Lib3dsMaterial *mat=0;
#ifdef	USE_SDL
                    Player_texture *pt = 0;
                    int tex_mode = 0;
#endif
                    if (f->material[0])
                    {
                        mat=lib3ds_file_material_by_name(m_model, f->material);
                    }

                    if( mat != oldmat )
                    {
                        if (mat)
                        {
                            if( mat->two_sided )
                                glDisable(GL_CULL_FACE);
                            else
                                glEnable(GL_CULL_FACE);

                            glDisable(GL_CULL_FACE);

                            /* Texturing added by Gernot < gz@lysator.liu.se > */

                            if (mat->texture1_map.name[0])
                            {		/* texture map? */
                                Lib3dsTextureMap *tex = &mat->texture1_map;
                                if (!tex->user.p)
                                {               /* no player texture yet? */
                                    char texname[1024];
                                    pt = (Player_texture *)malloc(sizeof(*pt));
                                    tex->user.p = pt;
                                    //snprintf(texname, sizeof(texname), "%s/%s", datapath, tex->name);
                                    strcpy(texname, datapath);
                                    strcat(texname, "/");
                                    strcat(texname, tex->name);
                                    IDE_DEBUG(QString("Loading texture map, name %1").arg(QLatin1String(texname)));
#ifdef	USE_SDL
#ifdef  USE_SDL_IMG_load
                                    pt->bitmap = IMG_load(texname);
#else
                                    pt->bitmap = IMG_Load(texname);
#endif /* USE_SDL_IMG_load */

#else /* USE_SDL */
                                    pt->bitmap = 0;
                                    IDE_DEBUG(QString("3dsplayer: Warning: No image loading support, skipping texture"));
#endif /* USE_SDL */
                                    if (pt->bitmap)
                                    {	/* could image be loaded ? */
                                        /* this OpenGL texupload code is incomplete format-wise!
                                        * to make it complete, examine SDL_surface->format and
                                        * tell us @lib3ds.sf.net about your improvements :-)
                                        */
#ifdef USE_SDL
                                        int upload_format = GL_RED; /* safe choice, shows errors */
                                        int bytespp = pt->bitmap->format->BytesPerPixel;
                                        void *pixel = 0;
                                        glGenTextures(1, &pt->tex_id);
#ifdef	DEBUG
                                        printf("Uploading texture to OpenGL, id %d, at %d bytepp\n"),
                                        pt->tex_id, bytespp);
#endif	/* DEBUG */
                                        if (pt->bitmap->format->palette)
                                        {
                                            pixel = convert_to_RGB_Surface(pt->bitmap);
                                            upload_format = GL_RGBA;
                                        }
                                        else
                                        {
                                            pixel = pt->bitmap->pixels;
                                            /* e.g. this could also be a color palette */
                                            if (bytespp == 1)
                                                upload_format = GL_LUMINANCE;
                                            else if (bytespp == 3)
                                                upload_format = GL_RGB;
                                            else if (bytespp == 4)
                                                upload_format = GL_RGBA;
                                        }
                                        glBindTexture(GL_TEXTURE_2D, pt->tex_id);
                                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                                    TEX_XSIZE, TEX_YSIZE, 0,
                                                    GL_RGBA, GL_UNSIGNED_BYTE, 0);
                                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                                        glTexParameteri(GL_TEXTURE_2D,
                                        GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                                        glTexParameteri(GL_TEXTURE_2D,
                                        GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                                        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                                        glTexSubImage2D(GL_TEXTURE_2D,
                                        0, 0, 0, pt->bitmap->w, pt->bitmap->h,
                                        upload_format, GL_UNSIGNED_BYTE, pixel);
                                        pt->scale_x = (float)pt->bitmap->w/(float)TEX_XSIZE;
                                        pt->scale_y = (float)pt->bitmap->h/(float)TEX_YSIZE;
#endif /* USE_SDL */
                                        pt->valid = 1;
                                    }
                                    else
                                    {
                                        IDE_DEBUG(QString("Load of texture %1 did not succeed (format not supported)").arg(QLatin1String(texname)));
                                        pt->valid = 0;
                                    }
                                }
                                else
                                {
                                    pt = (Player_texture *)tex->user.p;
                                }
                                tex_mode = pt->valid;
                            }
                            else
                            {
                                tex_mode = 0;
                            }
                            glMaterialfv(GL_FRONT, GL_AMBIENT, mat->ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat->diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, mat->specular);
                            glMaterialf(GL_FRONT, GL_SHININESS, pow(2, 10.0*mat->shininess));
                        }
                        else
                        {
                            static const Lib3dsRgba a={0.7, 0.7, 0.7, 1.0};
                            static const Lib3dsRgba d={0.7, 0.7, 0.7, 1.0};
                            static const Lib3dsRgba s={1.0, 1.0, 1.0, 1.0};
                            glMaterialfv(GL_FRONT, GL_AMBIENT, a);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, s);
                            glMaterialf(GL_FRONT, GL_SHININESS, pow(2, 10.0*0.5));
                        }
                        oldmat = mat;
                    }
                    else if(mat && mat->texture1_map.name[0])
                    {
                        Lib3dsTextureMap *tex = &mat->texture1_map;
                        if(tex && tex->user.p)
                        {
                            pt = (Player_texture *)tex->user.p;
                            tex_mode = pt->valid;
                        }
                    }

                    {
                        int i;
                        if (tex_mode)
                        {
                            //printf("Binding texture %d\n", pt->tex_id);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, pt->tex_id);
                        }

                        glBegin(GL_TRIANGLES);
                        glNormal3fv(f->normal);
                        for (i=0; i<3; ++i)
                        {
                            glNormal3fv(normalL[3*p+i]);

                            if(tex_mode)
                            {
                                glTexCoord2f(mesh->texelL[f->points[i]][1]*pt->scale_x,
                                pt->scale_y - mesh->texelL[f->points[i]][0]*pt->scale_y);
                            }

                            glVertex3fv(mesh->pointL[f->points[i]].pos);
                        }
                        glEnd();

                        if (tex_mode)
                            glDisable(GL_TEXTURE_2D);
                    }
                }

                free(normalL);
            }

            glEndList();
        }

        if (mesh->user.d)
        {
            Lib3dsObjectData *d;

            glPushMatrix();
            d=&node->data.object;
            glMultMatrixf(&node->matrix[0][0]);
            glTranslatef(-d->pivot[0], -d->pivot[1], -d->pivot[2]);
            glCallList(mesh->user.d);
            /* glutSolidSphere(50.0, 20,20); */
            glPopMatrix();
            if( flush )
                glFlush();
        }
    }
}

/*!
* Update information about a light.  Try to find corresponding nodes
* if possible, and copy values from nodes into light struct.
*/

void GraphicsOpenGL::UpdateLight(Lib3dsLight *l)
{
    Lib3dsNode *ln, *sn;

    ln = lib3ds_file_node_by_name(m_model, l->name, LIB3DS_LIGHT_NODE);
    sn = lib3ds_file_node_by_name(m_model, l->name, LIB3DS_SPOT_NODE);

    if( ln )
    {
        memcpy(l->color, ln->data.light.col, sizeof(Lib3dsRgb));
        memcpy(l->position, ln->data.light.pos, sizeof(Lib3dsVector));
    }

    if( sn )
        memcpy(l->spot, sn->data.spot.pos, sizeof(Lib3dsVector));
}

void GraphicsOpenGL::DrawBounds(Lib3dsVector tgt)
{
    double cx,cy,cz;
    double lx,ly,lz;

    lx = sx / 10.; ly = sy / 10.; lz = sz / 10.;
    cx = tgt[0]; cy = tgt[1]; cz = tgt[2];

    glDisable(GL_LIGHTING);
    glColor4fv(white);
    glBegin(GL_LINES);
    glVertex3f(bmin[0],bmin[1],bmin[2]);
    glVertex3f(bmax[0],bmin[1],bmin[2]);
    glVertex3f(bmin[0],bmax[1],bmin[2]);
    glVertex3f(bmax[0],bmax[1],bmin[2]);
    glVertex3f(bmin[0],bmin[1],bmax[2]);
    glVertex3f(bmax[0],bmin[1],bmax[2]);
    glVertex3f(bmin[0],bmax[1],bmax[2]);
    glVertex3f(bmax[0],bmax[1],bmax[2]);

    glVertex3f(bmin[0],bmin[1],bmin[2]);
    glVertex3f(bmin[0],bmax[1],bmin[2]);
    glVertex3f(bmax[0],bmin[1],bmin[2]);
    glVertex3f(bmax[0],bmax[1],bmin[2]);
    glVertex3f(bmin[0],bmin[1],bmax[2]);
    glVertex3f(bmin[0],bmax[1],bmax[2]);
    glVertex3f(bmax[0],bmin[1],bmax[2]);
    glVertex3f(bmax[0],bmax[1],bmax[2]);

    glVertex3f(bmin[0],bmin[1],bmin[2]);
    glVertex3f(bmin[0],bmin[1],bmax[2]);
    glVertex3f(bmax[0],bmin[1],bmin[2]);
    glVertex3f(bmax[0],bmin[1],bmax[2]);
    glVertex3f(bmin[0],bmax[1],bmin[2]);
    glVertex3f(bmin[0],bmax[1],bmax[2]);
    glVertex3f(bmax[0],bmax[1],bmin[2]);
    glVertex3f(bmax[0],bmax[1],bmax[2]);

    glVertex3f(cx-size/32, cy, cz);
    glVertex3f(cx+size/32, cy, cz);
    glVertex3f(cx, cy-size/32, cz);
    glVertex3f(cx, cy+size/32, cz);
    glVertex3f(cx, cy, cz-size/32);
    glVertex3f(cx, cy, cz+size/32);
    glEnd();


    glColor4fv(red);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(lx,0.0,0.0);
    glEnd();

    glColor4fv(green);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,ly,0.0);
    glEnd();

    glColor4fv(blue);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,lz);
    glEnd();

    glEnable(GL_LIGHTING);
}


void GraphicsOpenGL::DrawLight(const GLfloat *pos, const GLfloat *color)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glScalef(size/20, size/20, size/20);
    glCallList(lightList);
    glPopMatrix();
}
#endif

/*!
* Main display function; called whenever the scene needs to be redrawn.
*/
void GraphicsOpenGL::Display()
{
#ifdef SURRPORT_3D
    Lib3dsNode *c,*t;
    Lib3dsFloat fov;
    Lib3dsFloat roll;
    float tmpnear;
    float tmpfar;
    float dist;
    float *campos;
    float *tgt;
    Lib3dsMatrix M;
    Lib3dsCamera *cam;
    Lib3dsVector v;
    Lib3dsNode *p;

    if( (m_model) && (m_model->background.solid.use) )
    {
        glClearColor(m_model->background.solid.col[0],
                     m_model->background.solid.col[1],
                     m_model->background.solid.col[2], 1.0);
    }

    /* TODO: fog */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if( anti_alias )
        glEnable(GL_POLYGON_SMOOTH);
    else
        glDisable(GL_POLYGON_SMOOTH);


    if (!m_model)
        return;

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_model->ambient);

    c=lib3ds_file_node_by_name(m_model, camera, LIB3DS_CAMERA_NODE);
    t=lib3ds_file_node_by_name(m_model, camera, LIB3DS_TARGET_NODE);

    if( t )
        tgt = t->data.target.pos;

    if( c )
    {
        fov = c->data.camera.fov;
        roll = c->data.camera.roll;
        campos = c->data.camera.pos;
    }

    if ((cam = lib3ds_file_camera_by_name(m_model, camera)) == 0)
        return;

    tmpnear = cam->near_range;
    tmpfar = cam->far_range;

    if (!c || !t)
    {
        if( !c )
        {
            fov = cam->fov;
            roll = cam->roll;
            campos = cam->position;
        }
        if( !t )
            tgt = cam->target;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* KLUDGE alert:  OpenGL can't handle a near clip plane of zero,
    * so if the camera's near plane is zero, we give it a small number.
    * In addition, many .3ds files I've seen have a far plane that's
    * much too close and the model gets clipped away.  I haven't found
    * a way to tell OpenGL not to clip the far plane, so we move it
    * further away.  A factor of 10 seems to make all the models I've
    * seen visible.
    */
    if( tmpnear <= 0.0 )
        tmpnear = tmpfar * 0.001;

    gluPerspective( fov, 1.0*gl_width/gl_height, tmpnear, tmpfar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1.0,0,0);

    /* User rotates the view about the target point */

    lib3ds_vector_sub(v, tgt, campos);
    dist = lib3ds_vector_length(v);

    glTranslatef(0.,dist, 0.);
    glRotatef(view_rotx, 1., 0., 0.);
    glRotatef(view_roty, 0., 1., 0.);
    glRotatef(view_rotz, 0., 0., 1.);
    glTranslatef(0.,-dist, 0.);

    lib3ds_matrix_camera(M, campos, tgt, roll);
    glMultMatrixf(&M[0][0]);

    /* Lights.  Set them from light nodes if possible.  If not, use the
    * light objects directly.
    */
    {
        static const GLfloat a[] = {0.0f, 0.0f, 0.0f, 1.0f};
        static GLfloat c[] = {1.0f, 1.0f, 1.0f, 1.0f};
        static GLfloat p[] = {0.0f, 0.0f, 0.0f, 1.0f};
        Lib3dsLight *l;

        int li=GL_LIGHT0;
        for (l=m_model->lights; l; l=l->next)
        {
            glEnable(li);

            UpdateLight(l);

            c[0] = l->color[0];
            c[1] = l->color[1];
            c[2] = l->color[2];
            glLightfv(li, GL_AMBIENT, a);
            glLightfv(li, GL_DIFFUSE, c);
            glLightfv(li, GL_SPECULAR, c);

            p[0] = l->position[0];
            p[1] = l->position[1];
            p[2] = l->position[2];
            glLightfv(li, GL_POSITION, p);

            if (l->spot_light)
            {
                p[0] = l->spot[0] - l->position[0];
                p[1] = l->spot[1] - l->position[1];
                p[2] = l->spot[2] - l->position[2];
                glLightfv(li, GL_SPOT_DIRECTION, p);
            }
            ++li;
        }
    }

    if( show_object )
    {
        for (p=m_model->nodes; p!=0; p=p->next)
        {
            RenderNode(p);
        }
    }

    if( show_bounds )
        DrawBounds(tgt);

    if( show_cameras )
    {
        for( cam = m_model->cameras; cam ; cam = cam->next )
        {
            lib3ds_matrix_camera(M, cam->position, cam->target, cam->roll);
            lib3ds_matrix_inv(M);

            glPushMatrix();
            glMultMatrixf(&M[0][0]);
            glScalef(size/20, size/20, size/20);
            glCallList(cameraList);
            glPopMatrix();
        }
    }

    if( show_lights )
    {
        Lib3dsLight *light;
        for( light = m_model->lights; light ; light = light->next )
        DrawLight(light->position, light->color);
        glMaterialfv(GL_FRONT, GL_EMISSION, black);
    }

    glutSwapBuffers();
#endif
}

/*!
*
*/
void GraphicsOpenGL::Resize(int w, int h)
{
#ifdef SURRPORT_3D
    gl_width=w;
    gl_height=h;
    glViewport(0,0,w,h);
#endif
}


DesignOpenGL::DesignOpenGL(QGraphicsItem *parent) :
    GraphicsOpenGL(parent)
{
    QAction *tmpDefault = m_ActionGroup->addAction(tr("0"));
    for(int i=1;i<10;i++)
    {
        m_ActionGroup->addAction(QString("%1").arg(i));
    }
    tmpDefault->setChecked(true);
}

QList<QAction*> DesignOpenGL::GetActionList()
{
    QList<QAction*> tmpList;
    if(m_ActionGroup)
    {
        tmpList = m_ActionGroup->actions();
        //>@根据当前已有的资源使能响应功能Action
        for(int i=0;i<tmpList.count();i++)
        {
            QAction *tmpAction = tmpList.at(i);
            if(tmpAction == 0)
                continue;
            int tmpState = tmpAction->text().toInt();
            if(m_EffectRC.contains(tmpState))
                tmpAction->setEnabled(true);
            else
                tmpAction->setDisabled(true);
        }
    }
    return tmpList;
}

void DesignOpenGL::ExecAction(QAction *pAction)
{
    updateEffect(pAction->text().toInt());
}
