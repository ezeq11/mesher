#ifndef LAYER_INCLUDED
	#include <cstdint>
	
	#include <vector>
	
	extern "C" {
		#include <vecGL/vecTris.h>
	}
	
	#define LAYER_NAME_STRLEN 10
	
	enum layerType{
		LAYER_VERT,
		LAYER_GRID
	};
	
	class layer{
		private:
			// Meta
			bool isVisible = true;
			char name[LAYER_NAME_STRLEN + 1];
			
		public:
			// General Globals ---------------------
			layer();
			virtual ~layer();
			
			virtual enum layerType type() = 0;
			
			// Meta ---------------------------
			void visibilityToggle();
			bool visible();
			
			void nameSet(const char *newName);
			const char *nameGet();
			
			// Nearest Point ---------------------
			virtual bool nearestPoint_Find(unsigned int radius) = 0;
			virtual bool nearestPoint_Found() = 0;
			virtual void nearestPoint_Clear() = 0;
			
			virtual int16_t nearestPoint_X() = 0;
			virtual int16_t nearestPoint_Y() = 0;
			
			static class layer *withNearestPoint(class layer *layer1,class layer *layer2);
			static class layer *withNearestPoint(std::vector<class layer *> &layers);
			
			// Drawing ---------------------
			virtual void draw() = 0;
	};
	
	class vertLayer: public layer{
		public:
			enum viewType{
				VIEW_XY,
				VIEW_UV,
				VIEW_COLORS,
				VIEW_BONES
			};
			
		private:
			// Parameters
			unsigned int maxTris;
			
			// Buffers and Displays
			struct vecTrisBuf buffer,disp;
			struct vecTris *dispTris;
			bool modified;
			
			// View State
			enum viewType currentView;
			
			// Vertex Modifiers
			void (*vertModifier)(int16_t*,int16_t*);
			bool (*vertModifierEnabled)();
			
			// Nearest Elements
			unsigned int nearVert,nearTri;
			
			// Selection
			unsigned char *selVerts,*selTris;
			unsigned int selVertCount;
			
			// Pseudo-Macros ----------------------
			unsigned int BUFF_XYUV_COUNT();
			unsigned int BUFF_TBC_COUNT();
			
			bool NEARVERT_VALID();
			bool NEARTRI_VALID();
			
			unsigned int SEL_VERTS_COUNT();
			unsigned int SEL_TRIS_COUNT();
			
			// Utility methods ---------------------
			void init(unsigned int maxTriCount,enum viewType initView);
			void end();
			
			void copyTri(struct vecTrisBuf *src,unsigned int srcI,struct vecTrisBuf *dest,unsigned int destI);
			
		public:
			// General Globals ---------------------
			vertLayer(unsigned int maxTriCount,enum viewType initView);
			~vertLayer();
			
			enum layerType type();
			
			// View State ------------------
			void setView(enum viewType newView);
			
			// Vertex Modifiers ------------------
			void vertModifiers_Set(void (*mod)(int16_t*,int16_t*),bool (*modEnabled)());
			bool vertModifiers_Applicable();
			void vertModifiers_Apply();
			
			// Inherited ---------------------
			bool nearestPoint_Find(unsigned int radius);
			bool nearestPoint_Found();
			void nearestPoint_Clear();
			
			int16_t nearestPoint_X();
			int16_t nearestPoint_Y();
			
			static class vertLayer *withNearestPoint(std::vector<class vertLayer *> &layers);
			
			void draw();
			void draw(bool wireframe,bool showNearestPoint);
			
			// Selections ---------------------
			void selectVert_Nearest();
			void selectVert_All();
			void selectVert_Clear();
			void selectVert_SelectedTris();
			
			void selectVert_ByBone(unsigned char bone);
			void selectVert_ByColor(unsigned char color);
			
			void selectTri_Nearest();
			void selectTri_All();
			void selectTri_Clear();
			void selectTri_SelectedVerts();
			
			// Buffer Operations ---------------------
			void nearVert_SetColor(unsigned char color);
			void nearVert_SetBone(unsigned char bone);
			
			void tris_Add(int16_t x0,int16_t y0,int16_t x1,int16_t y1,int16_t x2,int16_t y2,unsigned char type);
			void tris_DeleteSelected();
	};
	
	class gridLayer: public layer{
		private:
			// Meta
			unsigned char divisions = 0;
			
			// Nearest Point
			int16_t nearX,nearY;
			bool isNear;
			
		public:
			// General Globals ---------------------
			gridLayer();
			~gridLayer();
			
			enum layerType type();
			
			void set(unsigned char divs);
			unsigned char get();
			
			// Inherited ---------------------
			bool nearestPoint_Find(unsigned int radius);
			bool nearestPoint_Found();
			void nearestPoint_Clear();
			
			int16_t nearestPoint_X();
			int16_t nearestPoint_Y();
			
			void draw();
	};
	
	#define LAYER_INCLUDED
#endif