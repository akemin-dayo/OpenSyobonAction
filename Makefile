PROG=	SyobonAction
SRCS=	main.cpp loadg.cpp DxLib.cpp

# Can be overriden on the command line
PLATFORM=	linux

# System-specific compiler/linker flags
.if "${PLATFORM}" == "freebsd"
CFLAGS+= -I/usr/include -I/usr/local/include -I/usr/local/include/SDL
LDFLAGS+= -L/usr/lib -L/usr/local/lib
.endif

.if "${PLATFORM}" == "linux"
CFLAGS+= -I/usr/include -I/usr/include/SDL
LDFLAGS+= -L/usr/lib
.endif

.if "${PLATFORM}" == "netbsd"
CFLAGS+= -I/usr/include -I/usr/pkg/include -I/usr/pkg/include/SDL
LDFLAGS+= -L/usr/lib -L/usr/pkg/lib -Wl,-rpath,/usr/pkg/lib
.endif

.if "${PLATFORM}" == "openbsd"
CFLAGS+= -I/usr/include -I/usr/local/include -I/usr/local/include/SDL
LDFLAGS+= -L/usr/lib -L/usr/local/lib
.endif

# Global compiler/linker flags
CFLAGS+= -D_REENTRANT -D_GNU_SOURCE=1 -D_THREAD_SAFE
LDFLAGS+= -lSDL -lSDL_gfx -lSDL_image -lSDL_mixer -lSDL_ttf -lstdc++ -lpthread

# Do not generate manual pages if there aren't any
MKMAN=	no

.include <bsd.prog.mk>
