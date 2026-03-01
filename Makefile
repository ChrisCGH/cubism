INCLUDES=-I/usr/lib64/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread
LIBS=-pthread   -lwx_gtk3u_xrc-3.2 -lwx_gtk3u_webview-3.2 -lwx_gtk3u_html-3.2 -lwx_gtk3u_qa-3.2 -lwx_gtk3u_adv-3.2 -lwx_gtk3u_core-3.2 -lwx_baseu_xml-3.2 -lwx_baseu_net-3.2 -lwx_baseu-3.2 
#INCLUDES=-I/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread
#INCLUDES=-I/usr/lib64/wx/include/gtk3-unicode-3.1 -I/usr/include/wx-3.1 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread
#LIBS=-pthread   -lwx_gtk3u_xrc-3.1 -lwx_gtk3u_webview-3.1 -lwx_gtk3u_html-3.1 -lwx_gtk3u_qa-3.1 -lwx_gtk3u_adv-3.1 -lwx_gtk3u_core-3.1 -lwx_baseu_xml-3.1 -lwx_baseu_net-3.1 -lwx_baseu-3.1 
#OPT=-O2 -g -pthread
OPT=-g -pthread
all : wxcube

wxcube : wxcube.o cube.o
	g++ -o wxcube $(OPT) $(LIBS) wxcube.o cube.o

wxcube.o : wxcube.cpp cube.h 3D.h Projector.h
	g++ -c $(OPT) $(INCLUDES) $(DEFINES) wxcube.cpp

cube.o : cube.cpp cube.h
	g++ -c $(OPT) $(INCLUDES) $(DEFINES) cube.cpp

clean:
	rm -f *.o wxcube
