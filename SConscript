from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add ncnn include path.
if GetDepend('ARCH_ARMV8'):
    path  = [cwd + '/include']
    lib_path = [cwd + '/lib']

# add src and include to group.
group = DefineGroup('ncnn', src, depend = ['PKG_USING_NCNN'], CPPPATH = path, LIBS = ['ncnn'], LIBPATH=lib_path)

Return('group')