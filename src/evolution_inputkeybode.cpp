#include "evolution_inputimplement.h"

using namespace EVOLUTION;
using namespace EVOLUTION::INPUT;


KeyboardController::KeyboardController(IDirectInput8* directinput, HWND hwnd){

	// �L�[�{�[�h�f�o�C�X���擾
	auto hr = directinput->CreateDevice(GUID_SysKeyboard, &this->mp_device, NULL);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}

	//DirectInput �f�o�C�X�̃f�[�^ �t�H�[�}�b�g��ݒ肵�܂��B
	hr = this->mp_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}
	//�f�o�C�X�̂��̃C���X�^���X�ɑ΂��ċ������x����ݒ肵�܂��B
	/*
	DISCL_BACKGROUND
	�A�v���P�[�V�����̓o�b�N�O���E���h �A�N�Z�X����K�v�Ƃ��܂��B
	�o�b�N�O���E���h �A�N�Z�X�����^������ƁA�֘A�����E�B���h�E���A�N�e�B�u �E�B���h�E�łȂ��Ƃ��ł��A�f�o�C�X�����ł��l���ł��܂��B

	DISCL_EXCLUSIVE
	�A�v���P�[�V�����͔r���A�N�Z�X����K�v�Ƃ��܂��B
	�r���A�N�Z�X�����^������ƁA�f�o�C�X���l������Ă���ԁA
	�f�o�C�X�̑��̃C���X�^���X�̓f�o�C�X�ւ̔r���A�N�Z�X�����擾�ł��Ȃ��Ȃ�܂��B
	�������A�ʂ̃A�v���P�[�V�������r���A�N�Z�X�����擾�����ꍇ�ł��A�f�o�C�X�ւ̔�r���A�N�Z�X���͏�ɗ^�����܂��B�A�v���P�[�V�������}�E�X�܂��̓L�[�{�[�h �f�o�C�X��r�����[�h�Ŋl�����Ă���ꍇ�́AWM_ENTERSIZEMOVE ���b�Z�[�W����� WM_ENTERMENULOOP ���b�Z�[�W���󂯎�����Ƃ��ɁA
	��Ƀf�o�C�X���������K�v������܂��B�������Ȃ���΁A���[�U�[�̓��j���[�𑀍삵����A�E�B���h�E�̈ړ���T�C�Y�ύX���s�����肷�邱�Ƃ��ł��܂���B

	DISCL_FOREGROUND
	�A�v���P�[�V�����̓t�H�A�O���E���h �A�N�Z�X����K�v�Ƃ��܂��B
	�t�H�A�O���E���h �A�N�Z�X�����^������ƁA
	�֘A�����E�B���h�E���o�b�N�O���E���h�Ɉړ������Ƃ��ɁA�f�o�C�X�͎����I�ɉ������܂��B

	DISCL_NONEXCLUSIVE
	�A�v���P�[�V�����͔�r���A�N�Z�X����K�v�Ƃ��܂��B�f�o�C�X�ɃA�N�Z�X���Ă��A
	�����f�o�C�X�ɃA�N�Z�X���Ă��鑼�̃A�v���P�[�V�����ɂ͊����܂���B
	DISCL_NOWINKEYWindows ���S �L�[�𖳌��ɂ��܂��B
	���̃t���O��ݒ肷��ƁA���[�U�[������ăA�v���P�[�V�������甲���o�����Ƃ��Ȃ��Ȃ�܂��B
	�������A�f�t�H���g�̃A�N�V���� �}�b�s���O ���[�U�[ �C���^�[�t�F�C�X (UI) ���\������Ă���Ƃ��� DISCL_NOWINKEY �Ɍ��ʂ͂Ȃ��A
	Windows ���S �L�[�͂��� UI ���\������Ă������ʏ�ǂ��蓮�삷��̂Œ��ӂ��Ă��������B
	*/
	hr = this->mp_device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 8; // Arbitary buffer size

	//�f�o�C�X�̓�����`����v���p�e�B��ݒ肵�܂��B
	hr = this->mp_device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if (FAILED(hr)){
        EVOLUTION_ASSERT(0);
	}
	//���̓f�o�C�X�ւ̃A�N�Z�X���l�����܂�
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
	//�}�E�X���̍X�V
	//�|�C���^Swap
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