#include "weather.h"
static Texture2D snow;
static Texture2D snowCloud;
static Texture2D rain;
static Texture2D rainCloud;
static Texture2D raspberry;
static Texture2D moon;
static Texture2D sun;


bool isRain(environment* env) { 
	return env -> data.waterLevel > HOT_TRESH;
}
bool isHot(environment* env) { 
	return env -> data.tempC > HOT_TRESH;
}

void initWeather(void) {
		// Snow
		 Image snowImage = LoadImage("images/snow.png");
		 ImageResizeNN(&snowImage, snowImage.width * SCALING_FACTOR, snowImage.height * SCALING_FACTOR);
		 snow = LoadTextureFromImage(snowImage);
		 // Snow Cloud
		 Image snowCloudImage = LoadImage("images/snow_clouds.png");
		 ImageResizeNN(&snowCloudImage, snowCloudImage.width * SCALING_FACTOR, snowCloudImage.height * SCALING_FACTOR);
		 Texture2D snowCloud = LoadTextureFromImage(snowCloudImage);
		 // Rain
		 Image rainImage = LoadImage("images/rain.png");
		 ImageResizeNN(&rainImage, rainImage.width * SCALING_FACTOR, rainImage.height * SCALING_FACTOR);
		 Texture2D rain = LoadTextureFromImage(rainImage);
		 // Rain Cloud
		 Image rainCloudImage = LoadImage("images/rain_clouds.png");
		 ImageResizeNN(&rainCloudImage, rainCloudImage.width * SCALING_FACTOR, rainCloudImage.height * SCALING_FACTOR);
		 Texture2D rainCloud = LoadTextureFromImage(rainCloudImage);
		 // Raspberry
		 Image raspberryImage = LoadImage("image/Raspberry.png");
		 Texture2D raspberry = LoadTextureFromImage(raspberryImage);
		 // Moon
		 Image moonImage = LoadImage("images/Moon.png");
		 ImageResizeNN(&moonImage, moonImage.width * SCALING_FACTOR, moonImage.height * SCALING_FACTOR);
		 Texture2D moon = LoadTextureFromImage(moonImage);
		 // Sun
		 Image sunImage = LoadImage("images/Sun.png");
		 ImageResizeNN(&sunImage, sunImage.width * SCALING_FACTOR, sunImage.height * SCALING_FACTOR);
		 Texture2D sun = LoadTextureFromImage(sunImage);
		 // Grass image handling
		 Image grassImage = LoadImage("images/output-onlinepngtools.png");
		 ImageResizeNN(&grassImage, grassImage.width * GRASS_SCALING_FACTOR, grassImage.height * GRASS_SCALING_FACTOR);
		 Texture2D grass = LoadTextureFromImage(grassImage);
		 
			      
}
