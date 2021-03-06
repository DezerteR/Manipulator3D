CXXFLAGS = -I c:\MinGW\include -std=c++1y -msse2  -mfpmath=sse -I C:\Python27\include -DBOOST_PYTHON_STATIC_LIB -DMS_WIN64
CXX = C:\MinGw\bin\g++.exe
BUILDDIR = bin
SRC = src
Editor = ./Editor
UTIL = ./Utils

OBJS = \
../bin/Utils.o \
../bin/FontRenderer.o \
../bin/IMGUI.o \
../bin/Camera.o \
../bin/Logger.o \
../bin/CFGparser.o \
../bin/BulletWorld.o \
../bin/Engine.o \
../bin/ResourceLoader.o \
../bin/JacobianTransposed.o \
../bin/Robot.o \
../bin/IInterpolator.o \
../bin/RobotController.o \
../bin/Widgets.o   \
../bin/Editor.o  \
../bin/Graph.o \
../bin/MoveCommandBuilder.o \
../bin/ExecuteCommandBuilder.o \
../bin/WaitCommandBuilder.o \
../bin/PropertyEditor-TabManager.o \
../bin/Robot-Commands.o \
../bin/PythonBindings.o \
../bin/Helper.o \
../bin/MAIN.o

LIBS = -lboost_system -lboost_filesystem -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath \
-lglew32 -lglfw3 -lgdi32 -lFreeImage -lglu32 -lopengl32 -lboost_python-mgw49-mt-d-1_56 -LC:\Python27\libs -lpython27


../bin/manipulator: $(OBJS)
	$(CXX) $(OBJS) -o ../bin/manipulator $(LIBS)

../bin/MAIN.o: MAIN.cpp $(UTIL)/Utils.h $(UTIL)/Camera.h $(UTIL)/IMGUI_V4.h JacobianTransposed.h CFGparser.h Engine.h Menu.h Robot.h Robot-Commands.h Widgets.h $(Editor)/PropertyEditor-TabManager.h RobotController.h $(Editor)/Editor.h
	$(CXX) -c $(CXXFLAGS) MAIN.cpp -o ../bin/MAIN.o

#-----------------------------------------------------

../bin/PropertyEditor-TabManager.o: $(Editor)/PropertyEditor-TabManager.cpp $(Editor)/PropertyEditor-TabManager.h $(UTIL)/IMGUI_V4.h Widgets.h $(Editor)/MoveCommandBuilder.h
	$(CXX) -c $(CXXFLAGS) -o2 $(Editor)/PropertyEditor-TabManager.cpp -o ../bin/PropertyEditor-TabManager.o

../bin/MoveCommandBuilder.o: $(Editor)/MoveCommandBuilder.cpp $(Editor)/MoveCommandBuilder.h $(UTIL)/IMGUI_V4.h Widgets.h
	$(CXX) -c $(CXXFLAGS) -o2 $(Editor)/MoveCommandBuilder.cpp -o ../bin/MoveCommandBuilder.o

../bin/ExecuteCommandBuilder.o: $(Editor)/ExecuteCommandBuilder.cpp $(Editor)/ExecuteCommandBuilder.h $(UTIL)/IMGUI_V4.h Widgets.h
	$(CXX) -c $(CXXFLAGS) -o2 $(Editor)/ExecuteCommandBuilder.cpp -o ../bin/ExecuteCommandBuilder.o

../bin/WaitCommandBuilder.o: $(Editor)/WaitCommandBuilder.cpp $(Editor)/WaitCommandBuilder.h $(UTIL)/IMGUI_V4.h Widgets.h
	$(CXX) -c $(CXXFLAGS) -o2 $(Editor)/WaitCommandBuilder.cpp -o ../bin/WaitCommandBuilder.o

../bin/Editor.o: $(Editor)/Editor.cpp $(Editor)/Editor.h Widgets.h $(UTIL)/IMGUI_V4.h
	$(CXX) -c $(CXXFLAGS) -o2  $(Editor)/Editor.cpp -o ../bin/Editor.o

#-----------------------------------------------------

../bin/Helper.o: Helper.cpp Helper.h
	$(CXX) -c $(CXXFLAGS) -o2  Helper.cpp -o ../bin/Helper.o

../bin/PythonBindings.o: PythonBindings.cpp PythonBindings.h
	$(CXX) -c $(CXXFLAGS)  PythonBindings.cpp -o ../bin/PythonBindings.o

../bin/Graph.o: Graph.cpp Graph.h Widgets.h $(UTIL)/IMGUI_V4.h
	$(CXX) -c $(CXXFLAGS) -o2  Graph.cpp -o ../bin/Graph.o

../bin/Widgets.o: Widgets.cpp Widgets.h $(UTIL)/IMGUI_V4.h
	$(CXX) -c $(CXXFLAGS) -o2  Widgets.cpp -o ../bin/Widgets.o

#-----------------------------------------------------

../bin/Robot-Commands.o: $(UTIL)/IMGUI_V4.h Robot-Commands.cpp Robot-Commands.h $(UTIL)/BaseStructs.h
	$(CXX) -c $(CXXFLAGS) -o3 Robot-Commands.cpp -o ../bin/Robot-Commands.o

#-----------------------------------------------------

../bin/JacobianTransposed.o: JacobianTransposed.cpp JacobianTransposed.h Robot.h JacobianMatrix.h $(UTIL)/Utils.h
	$(CXX) -c $(CXXFLAGS) -o3  JacobianTransposed.cpp -o ../bin/JacobianTransposed.o

../bin/RobotController.o: RobotController.cpp RobotController.h JacobianTransposed.h Robot.h IInterpolator.h
	$(CXX) -c $(CXXFLAGS) -o3  RobotController.cpp -o ../bin/RobotController.o

../bin/IInterpolator.o: IInterpolator.cpp IInterpolator.h Robot.h
	$(CXX) -c $(CXXFLAGS) -o3  IInterpolator.cpp -o ../bin/IInterpolator.o

#-----------------------------------------------------

../bin/Robot.o: Robot.cpp Robot.h
	$(CXX) -c $(CXXFLAGS) -o2  Robot.cpp -o ../bin/Robot.o

../bin/CFGparser.o: CFGparser.cpp CFGparser.h $(UTIL)/Utils.h
	$(CXX) -c $(CXXFLAGS) CFGparser.cpp -o ../bin/CFGparser.o

../bin/ResourceLoader.o: ResourceLoader.cpp ResourceLoader.h $(UTIL)/Utils.h BulletWorld.h $(UTIL)/BaseStructs.h
	$(CXX) -c $(CXXFLAGS) -o2 ResourceLoader.cpp -o ../bin/ResourceLoader.o

../bin/Engine.o: Engine.cpp Engine.h $(UTIL)/glUtils.h  glUtils.h Engine-DrawUtils.h Engine-Externals.h Engine-Ilumination.h $(UTIL)/Utils.h $(UTIL)/BaseStructs.h
	$(CXX) -c $(CXXFLAGS) Engine.cpp -o ../bin/Engine.o

# --------- BULLET -------------------------

../bin/BulletWorld.o: BulletWorld.cpp BulletWorld.h
	$(CXX) -c $(CXXFLAGS) -o2 BulletWorld.cpp -o ../bin/BulletWorld.o

# --------- UTILS -------------------------

../bin/Camera.o: $(UTIL)/Camera.cpp $(UTIL)/Camera.h $(UTIL)/Utils.h
	$(CXX) -c $(CXXFLAGS) -o2 $(UTIL)/Camera.cpp -o ../bin/Camera.o

../bin/IMGUI.o: $(UTIL)/IMGUI_V4.cpp $(UTIL)/IMGUI_V4.h $(UTIL)/Utils.h $(UTIL)/FontRenderer.h
	$(CXX) -c $(CXXFLAGS) -o2 $(UTIL)/IMGUI_V4.cpp -o ../bin/IMGUI.o

../bin/FontRenderer.o: $(UTIL)/FontRenderer.cpp $(UTIL)/FontRenderer.h $(UTIL)/Utils.h
	$(CXX) -c $(CXXFLAGS) -o2 $(UTIL)/FontRenderer.cpp -o ../bin/FontRenderer.o

../bin/Logger.o: $(UTIL)/Logger.cpp $(UTIL)/Logger.h
	$(CXX) -c $(CXXFLAGS) -o2 $(UTIL)/Logger.cpp -o ../bin/Logger.o

../bin/Utils.o: $(UTIL)/Utils.cpp $(UTIL)/Utils.h
	$(CXX) -c $(CXXFLAGS) -o2 $(UTIL)/Utils.cpp -o ../bin/Utils.o
