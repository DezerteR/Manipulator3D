﻿#pragma once
float cacheMx = 0.f;
float cacheMy = 0.f;
bool enableCameraZoom = false;

class Button {
public:
	string name;
	string image;
	HexColor mainColor;
	std::function<void(void)>fun;

	std::vector <Button> children;
	int mark;
	glm::vec4 position;
	bool marked;
	bool operator()(){

			for(int id=0; id<children.size(); id++){
				auto &it = children[id];
				if(it.image != ""){
					ui.image(it.image)
					// ui.rect(60, 20)
						.color(it.mainColor)(UI::Hoverable)
						.onlClick([id, &it, this](glm::vec4 r){
							mark = id;
							position = r;
							marked = true;
							it.fun();
						})
						.onHover([&it, id, this](glm::vec4 r){
							if (mark != id)ui.rect(r.x, r.y - 20, 100, 20).text(it.name)();
						});
				}
				else {
					ui.rect(150, 20)
						.color(it.mainColor).font("ui_12").text(it.name);
					if (it.children.size())ui.text(u"\ue0e3", "sym_12", UI::TextToRight);
					ui(UI::Hoverable).onlClick([id,&it,this](glm::vec4 r){
							mark = id;
							position = r;
							position.x += 150;
							position.y += 21;
							marked = true;
							it.fun();
						});
				}
			}

			ui.endTable();


			if(!marked)
				return marked;
			if(ui.lClick() && ui.outOfTable())
				marked = false;
			ui.table(position,UI::LayoutVertical | UI::AlignTop | UI::AlignLeft | UI::Draw);
				marked |= children[mark]();

			return marked;
	}
};

Button TopBar {"","",colorWhite, []{}, std::vector<Button>{
		Button {"Settings", "Settings", colorWhite,  []{}, vector<Button>{
			Button {"SSAO","",  colorWhite, []{globalSettings ^=  SSAO;}},
			Button {"Sobel","",  colorWhite, []{globalSettings ^=  SOBEL;}},
			Button {"Blur","",  colorWhite, []{globalSettings ^=  BLUR;}},
			Button {"Use downsample","",  colorWhite, []{globalSettings ^=  BLUR_WITH_DOWNSAMPLE;}},
			Button {"Graphic","", colorWhite, []{}},
			Button {"Lights","",  colorWhite, []{globalSettings ^= LIGHTS;}},
			Button {"Wireframe","",  colorWhite, []{}},
			Button {"Shadows","",  colorWhite, []{}},
			Button {"DOF","",  colorWhite, []{}},
			Button {"HDR","",  colorWhite, []{globalSettings ^= HDR;}},
			Button {"MSAA","",  colorWhite, []{globalSettings ^= MSAA;}},
			Button {"God Rays","",  colorWhite, []{}},
			Button {"Object Outlining","",  colorWhite, []{}}, // http://www.learnopengl.com/#!Advanced-OpenGL/Stencil-testing
			Button {"Colors","",  colorWhite, []{globalSettings ^=  DRAW_COLORS;}},
			Button {"Normals","",  colorWhite, []{globalSettings ^= DRAW_NORMALS;}},
			Button {"Depth","",  colorWhite, []{globalSettings ^=   DRAW_DEPTH;}},
			Button {"Lights","",  colorWhite, []{globalSettings ^=  DRAW_LIGHTS;}},
		}},
		Button {"Scene", "Scene", colorWhite, []{}, std::vector<Button>{
			Button {"Load","",  colorWhite, []{}},
			Button {"Reload","",  colorWhite, []{}},
			Button {"Clear","",  colorWhite, []{}},
			Button {"Prevent Collisions","",  colorWhite, []{}, std::vector<Button>{
				Button {"All", "", colorWhite, []{}},
				Button {"Some", "", colorWhite, []{}},
				Button {"None", "", colorWhite, []{}},
			}},
			Button {"Export Data","",  colorWhite, []{}},
			Button {"Grids","",  colorWhite, []{}, std::vector<Button>{
				Button {"XY", "", colorWhite, []{globalSettings = globalSettings ^ DRAW_XY_GRID;}},
				Button {"XZ", "", colorWhite, []{globalSettings = globalSettings ^ DRAW_XZ_GRID;}},
				Button {"YZ", "", colorWhite, []{globalSettings = globalSettings ^ DRAW_YZ_GRID;}},
			}},
		}},
		Button {"Script", "File", colorWhite, []{}, std::vector<Button>{
			Button {"Load","",  colorWhite, []{}},
			Button {"Reload","",  colorWhite, []{}},
			Button {"Clear","",  colorWhite, []{}},
			Button {"Run","",  colorWhite, []{}},
			Button {"Pause","",  colorWhite, []{}},
			Button {"Stop","",  colorWhite, []{}},
		}},
		Button {"Play F5", "Play", colorWhite, []{RC->run();}},
		Button {"Pause F6", "Pause", colorWhite, []{RC->pause();}},
		Button {"Stop", "Stop", colorWhite, []{RC->stop();}},
		Button{ "Undo", "Undo", colorWhite, []{RC->prev(); } },
		Button{ "Redo", "Redo", colorWhite, []{RC->next(); } },
		Button {"Save", "Save", colorWhite, []{}},
		Button {"Load", "Load", colorWhite, []{}},
}};

class MovableBar {
public:
	glm::vec4 position;
	glm::vec2 distance;
	bool isMoving = false;
	MovableBar(glm::vec4 pos): position(pos){}
	~MovableBar(){
		cerr<<"~MovableBar"<<endl;
	}
	void move(){
		isMoving = ui.onGroupGrab([this](glm::vec4){
			if(isMoving){
				position.x = mousePosition.x - distance.x;
				position.y = mousePosition.y - distance.y;
			}
			else {
				isMoving = true;
				distance = mousePosition - position.xy();
			}
			ui.captureMouse = true;
		});
	}

};

class SideBarPoints : public MovableBar  {
	bool minimized = true;
public:
	SideBarPoints(): MovableBar(glm::vec4(1400-160, 700-200,0,0)) {}
	~SideBarPoints(){
		cerr<<"~SideBarPoints"<<endl;
	}

	void operator ()(){
		ui.image(minimized?window_width-12:position.x+150, minimized?window_height-212:position.y-10, minimized?"Maximize"s:"Minimize"s)()
		.onlClick([this]{
			minimized = !minimized;
		});
		if(minimized)
			return;

		ui.table(position, UI::LayoutVertical | UI::AlignTop | UI::AlignLeft  | UI::FixedPos);
		ui.border(2);

			ui.box(UI::LayoutHorizontal | UI::AlignLeft );
			ui.border(1);
				ui.image("AddPoint"s)();
				ui.image("RemovePoint"s)();
				ui.image("UsePoint"s)();

			ui.endBox();

			ui.box(UI::LayoutVertical);
			ui.border(1);
				ui.rect(150,18).text("punkt 1"s).color(colorWhite).font("ui_12"s)();
				ui.rect(150,18).text("punkt 2"s).color(colorWhite).font("ui_12"s)();
				ui.rect(150,18).text("punkt 3"s).color(colorWhite).font("ui_12"s)();
				ui.rect(150,18).text("punkt 4"s).color(colorWhite).font("ui_12"s)();
			ui.endBox();
			move();
		ui.endTable();
	}

}sideBarPoints;

class PlusMinusWidget {
public:
	Module *module;
	float incrementValue = 2.f*toRad;
	PlusMinusWidget(Module *module): module(module){}

	void operator()(){
		// ui.box(UI::LayoutHorizontal | UI::AlignTop | UI::AlignLeft);
		ui.box(UI::LayoutVertical | UI::AlignTop | UI::AlignLeft);
		ui.image("Minimize")
			.onHover([this]{ module->entity->isOutlined = true; })
			.onlClick([this]{ module->value -=	incrementValue; })
			.onlPressed([this]{ if(signal10ms)module->value -=	incrementValue; })();
		ui.image("Maximize")
			.onHover([this]{ module->entity->isOutlined = true; })
			.onlClick([this]{ module->value +=	incrementValue; })
			.onlPressed([this]{ if(signal10ms)module->value +=	incrementValue; })();
		ui.rect(20,20)
			.text(std::to_string((int)(module->value*toDeg)))();
		ui.endBox();
	}

};

class SteeringConsole : public MovableBar {
	bool minimized = true;
public:
	vector<PlusMinusWidget> buttons;

	SteeringConsole():MovableBar({940,0,456,196}){}
	~SteeringConsole(){
		cerr<<"~SteeringConsole"<<endl;
	}

	void operator ()(){
		ui.image(minimized?window_width-10:position.x+440,minimized?12:position.y+198, minimized?"Maximize":"Minimize")()
		.onlClick([this]{
			minimized = !minimized;
		});
		if(minimized)
			return;
		ui.table(position, UI::LayoutHorizontal | UI::AlignBottom | UI::AlignLeft | UI::FixedSize | UI::FixedPos);
			ui.border(2);
			ui.box(UI::LayoutHorizontal).position(position + glm::vec4(20, 170, -456, -196));
			for(auto &it : buttons)
				it();
			ui.endBox();
			move();

		ui.endTable();

	}
}steeringConsole;

void recursiveTopMenu(Button button){


}


void MainMenu(){
	// top menu

	ui.rect(0, window_height-32, window_width, 32).color(0x3B3B3BFF)(UI::Label);
	// ui.table(UI::LayoutHorizontal | UI::AlignTop | UI::AlignLeft | UI::Draw);
	ui.table(UI::LayoutHorizontal | UI::AlignTop | UI::AlignLeft)
		.style(UI::StyleID::Dark);
		TopBar();
		// sam się zamyka

	ui.table(UI::LayoutHorizontal | UI::AlignTop | UI::AlignRight )
		.style(UI::StyleID::Dark);
	ui.image("Exit").color(colorRed)()
		.onlClick([&]{
			quit = true;
			})
		.onHover([](glm::vec4 r){
			ui.rect(r.x-10, r.y-20, 150,20).text("exit")(UI::CaptureMouse);
		});
	ui.endTable();

	// zrobic z tego scrollListę!

	// alert.show();
	sideBarPoints();
	steeringConsole();

	// if(ui.hasHover(glm::vec4(0,0,window_width, window_height)) &&	ui.lRepeat()){
		// float dx = (mouse_x - cacheMx)/window_width*2;
		// float dy = (cacheMy - mouse_y)/window_height*2;
		// camera.setMouseMov(dx, dy);
		// enableCameraZoom = true;
	// }
	// cacheMx = mouse_x;
	// cacheMy = mouse_y;


}
