#ifndef __EVOLUTION_INTERFACE_H__
#define __EVOLUTION_INTERFACE_H__

#include <evolution.h>
#include "evolution_inputnum.h"

namespace EVOLUTION{
    namespace INPUT{
        class IMouseController : public IUnknown{
            EVOLUTION_NOT_DESTRUCTOR(IMouseController);
        public:
            virtual bool IsButtonDown(MOUSE::BUTTON button, bool hold) = 0;
            virtual bool IsButtonUp(MOUSE::BUTTON button) = 0;
            virtual s32 GetMoveSpeed_X() = 0;
            virtual s32 GetMoveSpeed_Y() = 0;
            virtual s32 GetMoveSpeed_Z() = 0;
            virtual s32 GetMousePosX() = 0;
            virtual s32 GetMousePosY() = 0;
            virtual void Flip() = 0;
        };

        class IKeyboardController : public IUnknown{
            EVOLUTION_NOT_DESTRUCTOR(IKeyboardController);
        public:
            virtual bool IsButtonDown(KEY_BOARD::_BUTTON button, bool hold) = 0;
            virtual bool IsButtonUp(KEY_BOARD::_BUTTON button) = 0;
            virtual void Flip() = 0;
        };

        class IInputFactory : public IUnknown{
            EVOLUTION_NOT_DESTRUCTOR(IInputFactory);
        public:
            virtual s32 CreateMouseController(IMouseController** pp_mouse_controller) = 0;
            virtual s32 CreateKeyboardController(IKeyboardController** pp_keybode_controller) = 0;
        };
    }

    namespace FUNCTION{
        extern void CreateInputFactory(INPUT::IInputFactory** pp_input_factory, void* hwnd);
    }

    //-------------------------------------------------------
    //EVOLUTION Globally Unique Identifier
    //-------------------------------------------------------
    namespace EVOLUTION_GUID{
        // {04D3B871-4B9D-4445-A1D4-8DF4C8A53747}
        static const EVOLUTION_IID IID_IMouseController =
        { 0x4d3b871, 0x4b9d, 0x4445, { 0xa1, 0xd4, 0x8d, 0xf4, 0xc8, 0xa5, 0x37, 0x47 } };


        // {DF5366BC-583A-4b5c-B58A-62671F2215D6}
        static const EVOLUTION_IID IID_IKeyboardController =
        { 0xdf5366bc, 0x583a, 0x4b5c, { 0xb5, 0x8a, 0x62, 0x67, 0x1f, 0x22, 0x15, 0xd6 } };


        // {EC79014D-B743-431d-893A-F83608FFE0E9}
        static const EVOLUTION_IID IID_IInputFactory =
        { 0xec79014d, 0xb743, 0x431d, { 0x89, 0x3a, 0xf8, 0x36, 0x8, 0xff, 0xe0, 0xe9 } };
    }
}
#endif // !__INPUT_INTERFACE_H__
