# The simplest application example: 20 lines of code and yet all the power !

# A Viewer class is derived from QGLViewer and its <code>draw()</code> function is overloaded to
# specify the user defined OpenGL orders that describe the scene.

# This is the first example you should try, as it explains some of the default keyboard shortcuts
# and the mouse behavior of the viewer.

# This example can be cut and pasted to start the development of a new application.

TEMPLATE = app
TARGET   = simpleViewer

# Add your .ui file here
FORMS += untitled.ui

HEADERS  = simpleViewer.h \
    node.h \
    planete.h \
    rotation.h \
    translation.h \
    orbite.h \
    glm.h \
    vaisseau.h \
    courbebezier.h \
    particule.h \
    nuageparticules.h \
    vecteur3d.h \
    RT_particule.h \
    axexyz.h \
    pointcontrole.h \
    gestionpointscontrole.h \
    lignedroite.h

SOURCES  = simpleViewer.cpp main.cpp \
    node.cpp \
    planete.cpp \
    rotation.cpp \
    translation.cpp \
    orbite.cpp \
    glm.cpp \
    vaisseau.cpp \
    courbebezier.cpp \
    particule.cpp \
    nuageparticules.cpp \
    vecteur3d.cpp \
    RT_particule.cpp \
    axexyz.cpp \
    pointcontrole.cpp \
    gestionpointscontrole.cpp \
    lignedroite.cpp

QT *= xml opengl

# Might have to edit this to reflect the platform dependant name.
win32: LIBS += -lglut32
macx: LIBS += -framework GLUT

#CONFIG -= debug debug_and_release
CONFIG += debug_and_release qt opengl warn_on thread rtti console embed_manifest_exe

# --------------------------------------------------------------------------------------

# The remaining of this configuration tries to automatically detect the library paths.
# In your applications, you can probably simply use (see doc/compilation.html for details) :

#INCLUDEPATH *= C:/Users/debunne/Documents/libQGLViewer-2.3.6
#LIBS *= -LC:/Users/debunne/Documents/libQGLViewer-2.3.6/QGLViewer -lQGLViewer2

# Change these paths according to your configuration.

# --------------------------------------------------------------------------------------

win32 {
  LIB_FILE = QGLViewer
  CONFIG(debug, debug|release) {
    LIB_FILE = $$join(LIB_FILE,,,d)
  }
  LIB_FILE = $$join(LIB_FILE,,,2)
  LIBS += -l$${LIB_FILE}
}
macx {
  LIBS += -framework QGLViewer
}


