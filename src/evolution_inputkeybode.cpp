#include "evolution_inputimplement.h"

using namespace EVOLUTION;
using namespace EVOLUTION::INPUT;


KeyboardController::KeyboardController(IDirectInput8* directinput, HWND hwnd){

	// キーボードデバイスを取得
	auto hr = directinput->CreateDevice(GUID_SysKeyboard, &this->mp_device, NULL);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	//DirectInput デバイスのデータ フォーマットを設定します。
	hr = this->mp_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}
	//デバイスのこのインスタンスに対して協調レベルを設定します。
	/*
	DISCL_BACKGROUND
	アプリケーションはバックグラウンド アクセス権を必要とします。
	バックグラウンド アクセス権が与えられると、関連したウィンドウがアクティブ ウィンドウでないときでも、デバイスをいつでも獲得できます。

	DISCL_EXCLUSIVE
	アプリケーションは排他アクセス権を必要とします。
	排他アクセス権が与えられると、デバイスが獲得されている間、
	デバイスの他のインスタンスはデバイスへの排他アクセス権を取得できなくなります。
	ただし、別のアプリケーションが排他アクセス権を取得した場合でも、デバイスへの非排他アクセス権は常に与えられます。アプリケーションがマウスまたはキーボード デバイスを排他モードで獲得している場合は、WM_ENTERSIZEMOVE メッセージおよび WM_ENTERMENULOOP メッセージを受け取ったときに、
	常にデバイスを解放する必要があります。そうしなければ、ユーザーはメニューを操作したり、ウィンドウの移動やサイズ変更を行ったりすることができません。

	DISCL_FOREGROUND
	アプリケーションはフォアグラウンド アクセス権を必要とします。
	フォアグラウンド アクセス権が与えられると、
	関連したウィンドウがバックグラウンドに移動したときに、デバイスは自動的に解放されます。

	DISCL_NONEXCLUSIVE
	アプリケーションは非排他アクセス権を必要とします。デバイスにアクセスしても、
	同じデバイスにアクセスしている他のアプリケーションには干渉しません。
	DISCL_NOWINKEYWindows ロゴ キーを無効にします。
	このフラグを設定すると、ユーザーが誤ってアプリケーションから抜け出すことがなくなります。
	ただし、デフォルトのアクション マッピング ユーザー インターフェイス (UI) が表示されているときは DISCL_NOWINKEY に効果はなく、
	Windows ロゴ キーはその UI が表示されている限り通常どおり動作するので注意してください。
	*/
	hr = this->mp_device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 8; // Arbitary buffer size

	//デバイスの動作を定義するプロパティを設定します。
	hr = this->mp_device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}
	//入力デバイスへのアクセスを獲得します
	hr = this->mp_device->Acquire();
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	this->mp_primary = m_key[0];
	this->mp_secondary = m_key[1];

    memset(m_key, 0, sizeof(m_key));
}

u32 KeyboardController::AddRef(){
    return this->m_instance_counter.AddRef();
}

u32 KeyboardController::Release(){
    u32 counter = this->m_instance_counter.Release();
    if (counter == 0){
        EVOLUTION_RELEASE(this->mp_device);
        delete this;
    }
    return counter;
}

RESULT KeyboardController::QueryInterface(EVOLUTION::EVOLUTION_IID riid, void **ppvObject){

    if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IUnknown))
    {
        *ppvObject = static_cast<IUnknown *>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_IKeyboardController))
    {
        *ppvObject = static_cast<IKeyboardController*>(this);
        this->AddRef();
    }
    else if (EVOLUTION_EQUALGUID(riid, EVOLUTION_GUID::IID_KeyboardController))
    {
        *ppvObject = static_cast<KeyboardController*>(this);
        this->AddRef();
    }
    else
    {
        *ppvObject = nullptr;
        return RESULT::E_no_instance;
    }
    return RESULT::S_ok;
}

bool KeyboardController::IsButtonDown(KEY_BOARD::_BUTTON button, bool hold){
    if (!hold && this->mp_secondary[button] & 0x80)
    {
        return false;
    }
	return (this->mp_primary[button] & 0x80) == 0x80;
}

bool KeyboardController::IsButtonUp(KEY_BOARD::_BUTTON button){
	return ((this->mp_secondary[button] & 0x80) == 0x80 && this->mp_primary[button] == 0x00);
}

void KeyboardController::Flip(){
	//マウス情報の更新
	//ポインタSwap
	auto tmp = this->mp_secondary;
	this->mp_secondary = this->mp_primary;
	this->mp_primary = tmp;

	if (!this->mp_device){
		return ;
	}

	HRESULT hr = this->mp_device->GetDeviceState(sizeof(char) * 256, this->mp_primary);
	if (FAILED(hr))
	{
		hr = this->mp_device->Acquire();
		while (hr == DIERR_INPUTLOST){
			hr = this->mp_device->Acquire();
		}
		// Update the dialog text 
		if (hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED)
		{
			return ;
		}
	}
}