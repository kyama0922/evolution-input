#include "evolution_inputimplement.h"

using namespace EVOLUTION;
using namespace EVOLUTION::INPUT;


MouseController::MouseController(IDirectInput8* directinput, HWND hwnd):m_hwnd(hwnd){
	auto hr = directinput->CreateDevice(GUID_SysMouse, &this->mp_device, NULL);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	hr = this->mp_device->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}
	//デバイスのこのインスタンスに対して協調レベルを設定します。
	hr = this->mp_device->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	//
	DIPROPDWORD dipdw;
	memset(&dipdw, 0, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 16; // Arbitary buffer size

	//デバイスの動作を定義するプロパティを設定します。
	hr = this->mp_device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	//入力デバイスへのアクセスを獲得します
	hr = this->mp_device->Acquire();

	this->mp_primary = &this->m_mouse[0];
	this->mp_secondary = &this->m_mouse[1];
}

u32 MouseController::AddRef(){
	return this->m_instance_counter.AddRef();
}

u32 MouseController::Release(){
	u32 counter = this->m_instance_counter.Release();
	if (counter == 0){
		EVOLUTION_RELEASE(this->mp_device);
		delete this;
	}
	return counter;
}

RESULT MouseController::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){

    if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_IMouseController))
    {
        *ppvObject = static_cast<IMouseController*>(this);
        this->AddRef();
    }
    else if (IsEqualGUID(riid, EVOLUTION_GUID::IID_MouseController))
    {
        *ppvObject = static_cast<MouseController*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

bool MouseController::IsButtonDown(MOUSE::BUTTON button, bool hold){
    EVOLUTION_ASSERT(button < MOUSE::BUTTON8 + 1);
	if (!hold && this->mp_secondary->rgbButtons[button] & 0x80)
		return false;
	return (this->mp_primary->rgbButtons[button] & 0x80) ? true : false;
}

bool MouseController::IsButtonUp(MOUSE::BUTTON button){
    EVOLUTION_ASSERT(button < MOUSE::BUTTON8 + 1);
	return ((this->mp_secondary->rgbButtons[button] == 0x80) && (this->mp_primary->rgbButtons[button] == 0x00)) ? true : false;
}

s32 MouseController::GetMoveSpeed_X(){
	return this->mp_primary->lX;
}

s32 MouseController::GetMoveSpeed_Y(){
	return this->mp_primary->lY;
}

s32 MouseController::GetMoveSpeed_Z(){
	return this->mp_primary->lZ;
}

s32 MouseController::GetMousePosX(){
	return this->m_mousepos.x;
}

s32 MouseController::GetMousePosY(){
	return this->m_mousepos.y;
}

void MouseController::Flip(){
	//現在のマウスの座標を取得
	//GetCursorPos(&this->m_mouse_global_pos);
	//memcpy(&this->m_mousepos, &this->m_mouse_global_pos, sizeof(POINT));
	ScreenToClient(this->m_hwnd, &this->m_mousepos);

	//マウス情報の更新
	//ポインタSwap
	auto tmp = this->mp_secondary;
	this->mp_secondary = this->mp_primary;
	this->mp_primary = tmp;

	ZeroMemory(this->mp_primary, sizeof(DIMOUSESTATE2));
	if (!this->mp_device){
		return ;
	}

	HRESULT hr = this->mp_device->GetDeviceState(sizeof(DIMOUSESTATE2), this->mp_primary);

	if (FAILED(hr))
	{
		hr = this->mp_device->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = this->mp_device->Acquire();

		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
			return;
		}
	}
}