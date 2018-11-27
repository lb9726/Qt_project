#ifndef GRAPHICSINC_H
#define GRAPHICSINC_H

#include "graphicsscene.h"
#include "graphicsregion.h"
#include "graphicsarrow.h"
#include "graphicsfloor.h"
#include "graphicsmedia.h"
#include "graphicstext.h"
#include "graphicsclock.h"
#include "graphicsbutton.h"
#include "graphicsmessage.h"
#include "graphicsprogress.h"
#include "graphicskeyboard.h"
#ifdef SURRPORT_WEBKIT
#include "graphicswebview.h"
#endif
#ifdef SURRPORT_QML
#include "graphicsqml.h"
#endif
#ifdef SURRPORT_3D
#include "graphicsopengl.h"
#endif
#include "graphicsanimal.h"

#endif
