AssetDirs = [
"AnimationNameSets",
"AnimationSets",
"AnimationSpeedSets",
"IndexBuffers",
"GameObjectMetaScripts",
"GPUPrograms",
"Levels",
"Materials",
"MaterialSets",
"Meshes",
"NavigationGrids",
"NormalBuffers",
"PhysX",
"Scripts",
"Skeletons",
"Skins",
"SkinWeights",
"Sounds",
"TangentBuffers",
"TexCoordBuffers",
"Textures",
"VertexBuffers",
"PositionBuffers"
]

IgnoreDirs = [
".svn",
]

IgnoreExt = [
".gxp",
".sdb",
]

Client1LuaCommandServerPort = '1417'
Client2LuaCommandServerPort = '1418'
Client3LuaCommandServerPort = '1419'
Server1LuaCommandServerPort = '1500'

TargetSystems = [
    'localhost:'+Client1LuaCommandServerPort,
    'localhost:'+Client2LuaCommandServerPort,
    'XBox 360 RTH 2 (128.125.121.52):'+Client1LuaCommandServerPort,
    'PS3 RTH 1 (128.125.121.74):'+Client1LuaCommandServerPort,
    'PS3 RTH ToolKit (192.168.0.1):'+Client1LuaCommandServerPort,
    'PS3 RTH DevKit (192.168.0.5):'+Client1LuaCommandServerPort,
    'XBox 360 RTH 1 (128.125.121.51):'+Client1LuaCommandServerPort,
    'XBox 360 SAL (128.125.9.32):'+Client1LuaCommandServerPort,
    'PS3 SAL (128.125.3.21):'+Client1LuaCommandServerPort,
    'PS3 LA1 (192.168.1.4):'+Client1LuaCommandServerPort,
    'PS3 LA2 (192.168.1.3):'+Client1LuaCommandServerPort,
    'PS3 LA3 (192.168.1.2):'+Client1LuaCommandServerPort,
    'PS3 LA3 (192.168.1.1):'+Client1LuaCommandServerPort,
    'XBox 360 LA1 (192.168.0.5):'+Client1LuaCommandServerPort,
    'IPad (192.168.1.67):'+Client1LuaCommandServerPort,
	'Custom'
]

LaunchOptions = {
    'Basic' : {
        'platforms': {
            'Win32': [
                'Debug', 'Release',
            ],
            'xbox 360': [
                'Debug', 'Release',
            ]
        },
        'description': 'This executable is a shell for the engine. It does not initialize anything but the engine. Perfect for viewing assets via PyClient/AssetManager',
    },
    'CharacterControl' : {
        'platforms': {
            'Win32': [
                'Debug', 'Release',
            ],
            'xbox 360': [
                'Debug', 'Release',
            ]
        },
        'description': 'This is a demo of creating xustom animated game objects (soldiers), waypoints, animation, movement, behavior state machines. Have all of this work together using component data driven approach',
    },
    #add more executables as projects are added
}

ViewerConfiguration = "Debug_DX11"
ViewerDirectory = "Basic"
ViewerExecutable = "Basic.exe"

X360WorkspacePath = "devkit:\\PEW"


#files and folders ignored during project cloning (generation)
ProjectCloneIgnores = [
".svn",
".user",
".exe",
".obj",
".ncb",
]