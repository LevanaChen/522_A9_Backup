#ifndef __iosmainrenderview_h___
#define __iosmainrenderview_h___

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import <CoreFoundation/CFBase.h>

#   import <OpenGLES/EAGL.h>

#   import <OpenGLES/ES1/gl.h>
#   import <OpenGLES/ES1/glext.h>
#   import <OpenGLES/ES2/gl.h>
#   import <OpenGLES/ES2/glext.h>

// Inter-Engine includes
#include "PrimeEngine/MemoryManagement/Handle.h"
#include "PrimeEngine/Utils/PEClassDecl.h"

#include "PrimeEngine/Math/Vector4.h"
#include "PrimeEngine/Render/IRenderer.h"


// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface IOSMainRenderView: UIView
{
@public
    EAGLContext *context;
    
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view.
    GLuint defaultFramebuffer, colorRenderbuffer, depthBuffer;
}

@property (nonatomic, retain) EAGLContext *context;

- (void)setFramebuffer;
- (BOOL)presentFramebuffer;

@end

#endif