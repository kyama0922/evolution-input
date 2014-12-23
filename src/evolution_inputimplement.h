#ifndef __EVOLUTION_INPUT_IMPLEMENT_H__
#define __EVOLUTION_INPUT_IMPLEMENT_H__

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#include "../evolution_inputnum.h"
#include "../evolution_input_interface.h"

#ifndef NEW
#define NEW new
#endif

namespace EVOLUTION{
	namespace INPUT{

		class MouseController :public IMouseController{
		private:
			InstanceCounter m_instance_counter;
			IDirectInputDevice8* mp_device;
			HWND m_hwnd;
			DIMOUSESTATE2 m_mouse[2];
			DIMOUSESTATE2 *mp_primary, *mp_secondary;
			POINT m_mousepos;

		public:
			//IUnknown
			u32 AddRef();
			RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
			u32 Release();

			MouseController(IDirectInput8* directinput, HWND hwnd);
			bool IsButtonDown(MOUSE::BUTTON button, bool hold);
			bool IsButtonUp(MOUSE::BUTTON button);
			s32 GetMoveSpeed_X();
			s32 GetMoveSpeed_Y();
			s32 GetMoveSpeed_Z();
			s32 GetMousePosX();
			s32 GetMousePosY();
			void Flip();
		};

        class KeyboardController :public IKeyboardController{
		private:
			InstanceCounter m_instance_counter;
			IDirectInputDevice8* mp_device;
			char m_key[2][256];
			char *mp_primary, *mp_secondary;
		public:
			//IUnknown
            u32 AddRef();
			RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
			u32 Release();

            KeyboardController(IDirectInput8* directinput, HWND hwnd);
			bool IsButtonDown(KEY_BOARD::_BUTTON button, bool hold);
			bool IsButtonUp(KEY_BOARD::_BUTTON button);
			void Flip();
		};

		class InputFactory :public  IInputFactory{
		private:
			InstanceCounter m_instance_counter;
			IDirectInput8* mp_directinput;
			HWND m_hwnd;
		public:
			//IUnknown
			u32 AddRef();
			RESULT QueryInterface(EVOLUTION_IID riid, void **ppvObject);
			u32 Release();

			InputFactory(IDirectInput8* directinput, HWND hwnd);
			s32 CreateMouseController(IMouseController** pp_mouse_controller);
            s32 CreateKeyboardController(IKeyboardController** pp_keybode_controller);
		};


	}
    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {958DE907-59C0-4e0d-AE4C-D85394F66426}
        static const EVOLUTION_IID IID_MouseController =
        { 0x958de907, 0x59c0, 0x4e0d, { 0xae, 0x4c, 0xd8, 0x53, 0x94, 0xf6, 0x64, 0x26 } };

        // {4DE64876-1BA4-4189-A584-3887822BB1EB}
        static const EVOLUTION_IID IID_KeyboardController =
        { 0x4de64876, 0x1ba4, 0x4189, { 0xa5, 0x84, 0x38, 0x87, 0x82, 0x2b, 0xb1, 0xeb } };

        // {81F1CD49-D75C-4e90-B6B2-3766EEF1E2F2}
        static const EVOLUTION_IID IID_InputFactory =
        { 0x81f1cd49, 0xd75c, 0x4e90, { 0xb6, 0xb2, 0x37, 0x66, 0xee, 0xf1, 0xe2, 0xf2 } };
    }
}


#endif // !__INPUT_IMPLEMENT_H__