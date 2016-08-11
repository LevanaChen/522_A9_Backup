
#define NOMINMAX

// API Abstraction
#include "PrimeEngine/APIAbstraction/APIAbstractionDefines.h"

// Outer-Engine includes

// Inter-Engine includes

// Sibling/Children includes
#include "IOSApplication.h"
#include "PrimeEngine/Render/IOSMainRenderView.h"
#include "PrimeEngine/Game/Client/ClientGame.h"
#include "PrimeEngine/Game/Client/IOSGameRenderViewController.h"

namespace PE {

IOSApplication::IOSApplication(PE::GameContext &context, unsigned int width, unsigned int height, const char *caption)
{
}
	
void IOSApplication::exit()
{
}

Application* Application::Construct(PE::GameContext &context, unsigned int width, unsigned int height, const char *caption)
{
	Handle h("IOSApplication", sizeof(IOSApplication));
	IOSApplication *pApp = new (h) IOSApplication(context, width, height, caption);
	context.m_pApplication = pApp;
	return pApp;
}

}; // namespace PE


@implementation IOSAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// the Engine and Game have been initialised by now.
	// starting to catch os events
	
	// we could potentially do more initialization here..
	
    // application - is the object started from main with this object as event delegate
    
    // first we need window
    
	printf("PE: Progress: AppDelegate::didFinishLaunchingWithOptions Entry..\n");
    
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    
    IOSMainRenderView *baseView = [[[IOSMainRenderView alloc] init] autorelease];
    
    [window addSubview:baseView];
    baseView.multipleTouchEnabled = true;
    
    self.viewController = [[IOSGameRenderViewController alloc] init];
    
    self.viewController.m_pGameContext = &PE::Components::ClientGame::s_context;
    
    self.window.rootViewController = self.viewController; // will call viewController.viewDidLoad on this property assignment
    
    [self.window makeKeyAndVisible];
    
    //viewController.
    //[window addSubview:viewController.view];
    
    self.window.rootViewController = self.viewController;
    
    self.viewController.view = baseView;
    
    [self.viewController awakeFromNib];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// pause game (home button was pressed)
    [self.viewController pauseGame];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// the application was either just created or returned from paused
	[self.viewController unpauseGame];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.viewController pauseGame];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
}

- (void)dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end