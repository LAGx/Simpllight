#include "game_objects.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "phisic.h"
#include "log.h"
#include "window.h"
#include "state.h"
using namespace std;
using namespace sf;

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        BASE OBJECT       ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


BaseObject::BaseObject(Vector2f initCord, string textr) {
	if (!texture.loadFromFile(textr)) {
		throw Log::Exception("Texture load in BaseObject from " + textr, true);
	}
	texture.setSmooth(true);
	g_body.setTexture(texture);
	g_body.setOrigin(Vector2f(texture.getSize().x/2, texture.getSize().y/2));
	g_body.setPosition(initCord);
}

BaseObject::BaseObject() {	//empty//for inheritance
	isVisible = false;
}

void BaseObject::updateTextrPosition(Vector2f newCord, float newAngle) {
	g_body.setPosition(newCord);
	g_body.setRotation(newAngle);
}


void BaseObject::blit() {

	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender};
		spl::Window::allDrawable.push_back(draw);
	}
}

BaseObject::~BaseObject() {

}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////        DYNAMIC OBJECT       ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

DynamicObject::DynamicObject(b2World* World, Vector2f initCord, string textr, string name, figureType type, float figureSize, bool isSensor):BaseObject(initCord, textr){
	
	this->name = name;
	
	if ((texture.getSize().x - texture.getSize().y) && type != 3) 
		throw Log::Exception("Texture " + textr + " have to be square.");
	
	b2BodyDef b_bdef;
	b_bdef.type = b2_dynamicBody;
	b_bdef.linearDamping = 1;
	b_bdef.angularDamping = 1;
	b_bdef.position.Set(initCord.x / SCALE_BOX, initCord.y / SCALE_BOX);
	body_ph = World->CreateBody(&b_bdef);

	b2FixtureDef b_fixture;
	b2PolygonShape b_shape;
	b2CircleShape c_shape;
	b2Vec2 vec[6];
	float x = texture.getSize().x;
	float y = texture.getSize().y;
	switch (type) {
		case 1:{
			c_shape.m_radius = figureSize / SCALE_BOX;
			b_fixture.shape = &c_shape;
			break;
		}
		case 2: {
			vec[0].Set(0, -figureSize);
			vec[1].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			for (int i = 0; i < 3; i++) {
				vec[i].x = vec[i].x / SCALE_BOX;
				vec[i].y = vec[i].y / SCALE_BOX;
			}
			b_shape.Set(vec, 3);
			b_fixture.shape = &b_shape;
			break;
		}
		case 3: {
			vec[0].Set(-x/2+figureSize,y/2-figureSize);
			vec[1].Set(-x / 2 + figureSize, -y / 2 + figureSize);
			vec[2].Set(x / 2 - figureSize, -y / 2 + figureSize);
			vec[3].Set(x / 2 - figureSize, y / 2 - figureSize);
			for (int i = 0; i < 4; i++) {
				vec[i].x = vec[i].x / SCALE_BOX;
				vec[i].y = vec[i].y / SCALE_BOX;
			}
			b_shape.Set(vec, 4);
			b_fixture.shape = &b_shape;
			break;
		}
		case 4: {
			vec[0].Set(0, figureSize);
			vec[1].Set(-figureSize/ sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize)/ sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
			vec[2].Set(-figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
			vec[3].Set(figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
			vec[4].Set(figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
			for (int i = 0; i < 5; i++) {
				vec[i].x = vec[i].x / SCALE_BOX;
				vec[i].y = vec[i].y / SCALE_BOX;
			}
			b_shape.Set(vec, 5);
			b_fixture.shape = &b_shape;
			break;
		}
		case 5: {
			vec[0].Set(0, figureSize);
			vec[1].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			vec[3].Set(0, -figureSize);
			vec[4].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			vec[5].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
			for (int i = 0; i < 6; i++) {
				vec[i].x = vec[i].x / SCALE_BOX;
				vec[i].y = vec[i].y / SCALE_BOX;
			}
			b_shape.Set(vec, 6);
			b_fixture.shape = &b_shape;
			break;
		}
		default:
			throw Log::Exception("No such type (figure type) in DynamicObject");
	}

	b_fixture.isSensor = isSensor;
	b_fixture.density = 1;
	body_ph->CreateFixture(&b_fixture);
	body_ph->SetUserData(this);
}


void DynamicObject::setDrug(float linear, float angular) {
	body_ph->SetLinearDamping(linear);
	body_ph->SetAngularDamping(angular);
}

void DynamicObject::update() {
	updateTextrPosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX), body_ph->GetAngle()*DEG_BOX);
}

void DynamicObject::blit() {
	update();
	if (isVisible) {
		spl::ToDraw draw = { &g_body, depthRender };
		spl::Window::allDrawable.push_back(draw);
	}
}

DynamicObject::~DynamicObject() {

}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        STATIC OBJECT        ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

StaticObject::StaticObject(b2World* World, Vector2f initCord,float angle , string textr, string name, figureType type, float figureSize, bool isSensor) :BaseObject(initCord, textr) {

	this->name = name;

	if ((texture.getSize().x - texture.getSize().y) && type != 3)
		throw Log::Exception("Texture " + textr + " have to be square.");

	b2BodyDef b_bdef;
	b_bdef.type = b2_staticBody;
	b_bdef.position.Set(initCord.x / SCALE_BOX, initCord.y / SCALE_BOX);
	body_ph = World->CreateBody(&b_bdef);

	b2FixtureDef b_fixture;
	b2PolygonShape b_shape;
	b2CircleShape c_shape;
	b2Vec2 vec[6];
	float x = texture.getSize().x;
	float y = texture.getSize().y;
	switch (type) {
	case 1: {
		c_shape.m_radius = figureSize / SCALE_BOX;
		b_fixture.shape = &c_shape;
		break;
	}
	case 2: {
		vec[0].Set(0, -figureSize);
		vec[1].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 3; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 3);
		b_fixture.shape = &b_shape;
		break;
	}
	case 3: {
		vec[0].Set(-x / 2 + figureSize, y / 2 - figureSize);
		vec[1].Set(-x / 2 + figureSize, -y / 2 + figureSize);
		vec[2].Set(x / 2 - figureSize, -y / 2 + figureSize);
		vec[3].Set(x / 2 - figureSize, y / 2 - figureSize);
		for (int i = 0; i < 4; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 4);
		b_fixture.shape = &b_shape;
		break;
	}
	case 4: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		vec[2].Set(-figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[3].Set(figureSize / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)), (-tan(54.f / DEG_BOX)*figureSize) / sqrt(1 + tan(54.f / DEG_BOX)*tan(54.f / DEG_BOX)));
		vec[4].Set(figureSize / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)), (tan(18.f / DEG_BOX)*figureSize) / sqrt(1 + tan(18.f / DEG_BOX)*tan(18.f / DEG_BOX)));
		for (int i = 0; i < 5; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 5);
		b_fixture.shape = &b_shape;
		break;
	}
	case 5: {
		vec[0].Set(0, figureSize);
		vec[1].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[2].Set(-figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[3].Set(0, -figureSize);
		vec[4].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		vec[5].Set(figureSize / sqrt(1 + tan(PI / 6)*tan(PI / 6)), (-tan(PI / 6)*figureSize) / sqrt(1 + tan(PI / 6)*tan(PI / 6)));
		for (int i = 0; i < 6; i++) {
			vec[i].x = vec[i].x / SCALE_BOX;
			vec[i].y = vec[i].y / SCALE_BOX;
		}
		b_shape.Set(vec, 6);
		b_fixture.shape = &b_shape;
		break;
	}
	default:
		throw Log::Exception("No such type (figure type) in StaticObject");
	}

	b_fixture.isSensor = isSensor;
	body_ph->CreateFixture(&b_fixture);
	body_ph->SetTransform(body_ph->GetPosition(), angle/DEG_BOX);
	body_ph->SetUserData(this);
	updateTextrPosition(Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX), body_ph->GetAngle()*DEG_BOX);
}

StaticObject::~StaticObject() {

}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////        CURSOR               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Cursor::Cursor(b2World* World, std::string textr, std::string name) :DynamicObject(World, sf::Vector2f(0, 0), textr, name, circle_T, 1, true) {
	depthRender = -2000;
	setDrug(0,0);
}

void Cursor::positionMouse(int x, int y) {
	spl::Window::updateWindowStateBox();
	body_ph->SetTransform(b2Vec2((x/ spl::WindowStateBox::absoluteScale + spl::WindowStateBox::inGameZeroCordRelativeWindow.x) / SCALE_BOX, (y/ spl::WindowStateBox::absoluteScale + spl::WindowStateBox::inGameZeroCordRelativeWindow.y) / SCALE_BOX), 0);
	BaseObject::g_body.setScale(1/ spl::WindowStateBox::absoluteScale, 1 / spl::WindowStateBox::absoluteScale);
}

sf::Vector2f Cursor::getPosition() {
	return sf::Vector2f(body_ph->GetPosition().x*SCALE_BOX, body_ph->GetPosition().y*SCALE_BOX);
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////       PHANTOM               ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////


PhantomObject::PhantomObject(sf::Vector2f initCord, std::string texture) : BaseObject(initCord, texture) {
	g_body.setColor(sf::Color(255, 255, 255, 120));
	depthRender = -200;
}



void PhantomObject::setPosition(int x, int y, float angle) {
	updateTextrPosition(sf::Vector2f(x, y),angle);
}

sf::Vector3f PhantomObject::getPosition() {
	return sf::Vector3f(g_body.getPosition().x, g_body.getPosition().y, g_body.getRotation());
}