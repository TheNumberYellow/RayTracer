#include <vector>

#include "EasyBMP.h"

#include "MathUtils.h"

#include <Windows.h>


struct Sphere {
	Vector pos;
	double radius = 1.0;
	Colour col = { 1.0, 1.0, 1.0, 1.0 };
};

struct LightSource {
	LightSource() : pos(0.0, 0.0, 0.0), col(1.0, 1.0, 1.0) {}
	LightSource(Vector position) : pos(position), col(1.0, 1.0, 1.0) {}
	LightSource(Vector position, Colour colour) : pos(position), col(colour) {}


	Vector pos;
	Colour col = { 1.0, 1.0, 1.0, 1.0 };
};

struct World {
	
	std::vector<Sphere> spheres;
	std::vector<LightSource> lights;

	Colour sendRay(Vector from, Vector direction) {
		Colour finalColour;
		
		bool hit = false;
		Sphere hitSphere;
		Vector hitPoint;
		for (int i = 0; i < spheres.size(); i++) {

			double t0, t1;

			Vector L = spheres[i].pos - from;
			double tca = Vector::DotProduct(L, direction);
			if (tca < 0) continue;
			double d2 = Vector::DotProduct(L, L) - (tca * tca);
			if (d2 > (spheres[i].radius * spheres[i].radius)) continue;
			double thc = sqrt((spheres[i].radius * spheres[i].radius) - d2);
			t0 = tca - thc;
			t1 = tca + thc;

			if (t0 > t1) std::swap(t0, t1);

			if (t0 < 0) {
				t0 = t1;
				if (t0 < 0) continue;
			}
			hitSphere = spheres[i];
			hit = true;
			hitPoint = (direction * t0) - from;
		}
		
		if (hit) {
			for (int i = 0; i < lights.size(); i++) {

				//TODO: Averages is how imma do light colours

				Vector lightSource = lights[i].pos;
				double deflection = Vector::DotProduct(Vector::Normalize(hitPoint - hitSphere.pos), Vector::Normalize(lightSource - hitPoint));

				double r = ((hitSphere.col.r) + (lights[i].col.r)) / 2.0;
				double g = ((hitSphere.col.g) + (lights[i].col.g)) / 2.0;
				double b = ((hitSphere.col.b) + (lights[i].col.b)) / 2.0;

				r *= deflection;
				g *= deflection;
				b *= deflection;

				r = Math::Clamp(r, 0.0, 1.0);
				g = Math::Clamp(g, 0.0, 1.0);
				b = Math::Clamp(b, 0.0, 1.0);

				finalColour = finalColour + Colour(r, g, b, hitSphere.col.a);
			}
		}
		
		return finalColour;
	}


};



int main() {
	
	int imageWidth = 2000;
	int imageHeight = 2000;

	Camera cam;
	cam.pos = { 0.0f, 0.0f, 0.0f };
	cam.dir = { 1.0f, 0.0f, 0.0f };

	World world;
	world.spheres.push_back(Sphere{ {4.0f, 0.0f, 0.0f}, 0.6f, {1.0, 0.0, 0.0, 1.0} });
	world.spheres.push_back(Sphere{ {4.0f, -1.2f, -0.2f}, 0.3f, {0, 1.0, 0, 1.0} });
	world.spheres.push_back(Sphere{ {4.0f, 1.2f, 0.2f}, 0.3f, {0, 0, 1.0, 1.0} });
	world.spheres.push_back(Sphere{ {4.0f, -1.0f, -0.8f}, 0.2f, {0.0, 1.0, 1.0, 1.0} });
	world.spheres.push_back(Sphere{ {4.0f, 1.0f, 0.8f}, 0.2f, {1.0, 1.0, 0.0, 1.0} });


	world.lights.push_back(LightSource(Vector(0.0, 0.0, -8.0), Colour(0.3, 0.3, 0.3)));
	world.lights.push_back(LightSource(Vector(8.0, 0.0, 4.0), Colour(0.2, 0.2, 0.2)));
	world.lights.push_back(LightSource(Vector(0.0, 0.0, 0.0), Colour(0.01, 0.01, 0.01)));


	BMP output;
	output.SetSize(imageWidth, imageHeight);
	

	for (int y = 0; y < imageHeight; y++) {
		if (y % (imageHeight / 10) == 0) {
			std::cout << "" << ((float)y / imageHeight) * 100 << " percent.\n";
		}
		for (int x = 0; x < imageWidth; x++) {
			
			double ratioHorizontal = ((double)(x + 0.5) / (double)imageWidth) - 0.5;
			double ratioVertical = ((double)(y + 0.5) / (double)imageHeight) - 0.5;
			
			Vector point = Vector(1.0, ratioHorizontal, ratioVertical);

			Vector ray = Vector::Normalize(point - cam.pos);
			
			Colour pixelColour = world.sendRay(cam.pos, ray);
			
			// Order of colours in RGBApixel is blue green red alpha
			output.SetPixel(x, y, RGBApixel{ (unsigned char)(pixelColour.b * 255.0), (unsigned char)(pixelColour.g * 255.0), (unsigned char)(pixelColour.r * 255.0), (unsigned char)(pixelColour.a * 255.0) });
		}
	}
	
	
	
	output.WriteToFile("testfile.bmp");
	ShellExecute(0, 0, "testfile.bmp", 0, 0, SW_SHOW);
	return 0;
}