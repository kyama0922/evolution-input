#include "evolution_inputimplement.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

using namespace EVOLUTION;
using namespace EVOLUTION::INPUT;

void EVOLUTION::FUNCTION::CreateInputFactory(IInputFactory** pp_input_factory, void* hwnd){
	*pp_input_factory = nullptr;
	static struct _I{
		IDirectInput8* mp_directinput;
		_I() :mp_directinput(nullptr){
			// DirectInput objectì¬
			auto hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&this->mp_directinput, NULL);
			if (FAILED(hr)){
				EVOLUTION_ASSERT(0);
			}
		}
		~_I(){
			EVOLUTION_RELEASE(this->mp_directinput);
		}
	}_i;

	InputFactory* factory = new InputFactory(_i.mp_directinput, (HWND)hwnd);
	*pp_input_factory = factory;
}

InputFactory::InputFactory(IDirectInput8* directinput, HWND hwnd) :mp_directinput(directinput) , m_hwnd(hwnd){

}

u32 InputFactory::AddRef(){
    return this->m_instance_counter.AddRef();
}

u32 InputFactory::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        this->mp_directinput = nullptr;
        delete this;
    }
    return counter;
}

RESULT InputFactory::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){

    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IInputFactory))
    {
        *ppvObject = static_cast<IInputFactory*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_InputFactory))
    {
        *ppvObject = static_cast<InputFactory*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

s32 InputFactory::CreateMouseController(IMouseController** pp_mouse_controller){
	*pp_mouse_controller = nullptr;
	MouseController* mouse = new MouseController(this->mp_directinput, this->m_hwnd);
	*pp_mouse_controller = mouse;
	return 0;
}

s32 InputFactory::CreateKeyboardController(IKeyboardController** pp_keybode_controller){
	*pp_keybode_controller = nullptr;
	KeyboardController* keybode = new KeyboardController(this->mp_directinput, this->m_hwnd);
	*pp_keybode_controller = keybode;
	return 0;
}
