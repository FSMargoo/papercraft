## 前言

我以及给所有类和方法都添加了相对详细的注释，遇到本文档没能涉及到的，且依然有疑问的，**请参考代码中的注释**。

并且，我没有发现一些**隐藏的 bug** 以及一些 API 可能会有**功能缺失**的情况，请接手同学做好自行扩充修改的准备。

最后，希望你能够**认真阅读完本文档，这对你接手我的代码真的很重要**。

*我的时间实在有限，无法将所有设计都考虑周全，希望你能批评性地阅读我的源码，不足之处及时改正才能避免项目崩坏。*


## 我的代码喜欢

我的代码会在一些不可扭转的严重错误中抛出异常（继承于 ```std::exception``` 的 C++ 标准库异常）。因此你可以看到 ```main.cpp``` 中如此写道：

```C++
try
{
#ifndef _DEBUG
    PLanuncher launcher;
    launcher.Loop();
    auto gameSize = launcher.GetGameScreenSize();
    PLGame gameWindow(std::get<0>(gameSize), std::get<1>(gameSize), launcher.GetPlayerName());
    gameWindow.Loop();
#else
    PLGame gameWindow(640, 480, "Developer");
    gameWindow.Loop();
#endif
}
catch (std::exception &e)
{
    printf("Sorry, but PaperCraft was crashed with information:\n%s\n", e.what());
}
```

希望你也可以**善用 C++ 的异常，并在关键地方抛出异常并给足相关调试消息**。

除此之外，我的代码也喜欢提供抽象接口以供在没有完整实现的时候也能现在本模块实现该功能。例如 ```include/gui/GUIObject.h``` 中的 ```PGUIMangerInterface```：

```C++
/// The interface base class for GUI manager
class PGUIMangerInterface
{
public:
    PGUIMangerInterface() = default;
    virtual ~PGUIMangerInterface() = default;

public:
    /// Raise up a object as the focusing object in the manager
    /// @param Object The object to be raised
    virtual void RaiseAsFocus(PGUIObject* Object) = 0;
    /// Remove the object from the focus object
    /// @param Object The object be removed
    virtual void UnfocusObject(PGUIObject* Object) = 0;
    /// Get the focusing object pointer
    /// @return If the manager has not focusing object, it will return nullptr
    virtual PGUIObject* GetFocusingObject() = 0;
};
```

我认为这个是一个好习惯，因为你可以借此来预支没有实现的功能，并且把这个功能的具体实现交给下一个接力的同学（如果有必要的话）。

## 我对项目结构的规划

打开项目，你可以看到一个比较清晰地对项目结构的规划。我将 Game 文件夹以外的模块都称之为“Paper Library”，这也是为什么你看到的所有 API 都会以 P 作为首字母开头。如果可以，我希望你也能保持这个习惯以保持整个项目结构命名统一。

## 我干了什么

1. 总的来说，我编写了一套游戏的基本所需要的框架，例 UI 库/MCI 管理器/Asset 管理器。
2. MCI 管理器：我编写了一套 MCI 管理器，以方便 MP3 音频的管理与播放。下面是一个利用 MCI 管理器播放音频的例子：

```C++
#include "include/mic/MCIManager.h"

int main()
{
    // Open a demo mp3 file
    auto instance = PMCIManager::OpenBGM("./README/demo.mp3", "Test");
    // Play a sound and waiting for end
    PMCIManager::PlayWait(instance, nullptr);

    return 0;
}
```

MCI 管理器不仅支持简单的音频播放，还支持循环播放，例如你可以使用：

```C++
PMCIManager::Play(instance, nullptr, true);
```

来播放音频，但请注意 ```PMCIManager::PlayWait``` 并不支持 repeat 播放。

如果你希望通过一个新的线程来播放你的音频文件，可以像下面这样：

```C++
PMCIManager::PlayAndClose("./README/demo.mp3", "Test");
```

函数 ```PMCIManager::PlayAndClose``` 将会直接在新的线程中播放音频，并会在音频结束后自动销毁相关资源。

不仅如此，MCI 管理器还支持体积音效。即通过玩家摄像机的位置与声源位置自动调整大小（越靠近声源，声音越大，反之越小）。但使用该功能需要后续的同学在相机类中继承实现一个抽象类 ```PMCIManagerCameraInterface``` 的接口。在源码中，```PMCIManagerCameraInterface``` 的定义如下：

```C++
/// The abstracted class for MCI manager to call the API from
/// a specified camera
class PMCIManagerCameraInterface
{
public:
    /// Get the Y position of the camera
    /// @return The Y position
    virtual int GetY() const = 0;
    /// Get the X position of the camera
    /// @return The X position
    virtual int GetX() const = 0;
};
```

即需要相机覆写这两个方法以便向播放器提供玩家相机位置。假设已经有了一个 ```camera``` 对象，就可以通过 ```PMCIManagerCameraInterface``` 指针来传入相机信息给 MCI 管理器了，那么播放体积音效的代码应该这么写：

```C++
// X, Y is the position of the volume source
auto instance = PMCIManager::Open(L"./assets/sound/SFX.mp3", L"SFX", X, Y);

PMCIManager::PlayWait(instance, camera);
PMCIManager::Close(instance);
```

以及你可以通过 ```PMCIManager::Open(L"./assets/sound/SFX.mp3", L"SFX", X, Y, Range)``` 来指定 ```Range``` 为衰减半径。

注意，此处不可使用 ```PMCIManger::Play``` 来播放环境音效，```PMCIManger::Play``` 中的 ```PMCIManagerCameraInterface``` 仅作为保留参数，默认值为 ```nullptr```。

值得注意的是，由于 MCI 管理器的底层是 ```Win32 API```，我强烈建议你**对所有需要的音频文件都转换为 mp3 格式并去除 mp3 的封面**。

需要注意一点，对于 ```PMCIInstance```，每一个实例在播放后都会失效，比如下面的代码：

```C++
#include "include/mic/MCIManager.h"

int main()
{
    // Open a demo mp3 file
    auto instance = PMCIManager::OpenBGM("./README/demo.mp3", "Test");
    // Play a sound and waiting for end
    PMCIManager::PlayWait(instance, nullptr);

    // Play again
    PMCIManager::PlayWait(instance, nullptr);

    return 0;
}
```

第二次调用 ```PMCIManager::PlayWait``` 时，MCI 管理器将不会再次播放音频，这是因为实例已经失效。如果你尝试运行这段代码，你会发现  ```PMCIManager::PlayWait``` 会抛出一个 ```PMCIManagerFailureInvalidInstance``` 异常。更新实例有两种方法，下面的代码将展示这两种方法：
```C++
#include "include/mic/MCIManager.h"

int main()
{
    // Open a demo mp3 file
    auto instance = PMCIManager::OpenBGM("./README/demo.mp3", "Test");
    // Play a sound and waiting for end
    PMCIManager::PlayWait(instance, nullptr);

    // Self update, okay
    instance->Update();

    // Play again
    PMCIManager::PlayWait(instance, nullptr);

    // Updated by MCI manager, okay
    instance = PMCIManager::Update(instance);

    return 0;
}
```

3. 针对 EasyX API 的 wrapper：在 include/EasyXBase.h 下，你可以看到一些对于 EasyX 的封装类。例如 ```PImage``` 和 ```PDevice``` 及 ```PWindow```。

这里简单介绍一下 ```PDevice```，```PDevice``` 作为设备类，提供了一个统一的 API 来进行设备操作。```PDevice``` 仅支持从静态成员函数中创建，你可以通过 ```PDevice::MakeFromImage``` 以及 ```PDevice::MakeFromScreen``` 。具体的的使用方法请参考代码注释。

值得一提的是 ```PImage``` 添加了路径自动检查功能，当你通过 ```PImage``` 加载了一个不存在的路径（或者是不受支持的文件类型时），```PImage``` 将会在构造函数内抛出 ```PImageCreationFailure```，并在 ```std::exception::what``` 中详细告知是何路径导致了此异常，与此类似的还有 ```PDeviceCreationFailure```。

4. UI 系统：如你所见，我搭建了一套较为完整的 UI 系统，下面有一个例子很直观地展示了如何去使用这个 UI 库：

```c++
/// The example class for UI creation
class UIExample
{
public:
    UIExample()
    {
        /// Create a window in specified size
        PWindow window(429, 365, "UI Example");

        // Create a GUI control manager to control repaint progress and message loop
        PGUIManager manager;

        // Get the device wrapper of the PWindow
        //      Actually, you can create a PImage's device by the function:
        //            (static function) PDevice::MakeFromImage
        auto device = window.GetDevice();

        // Createing UI control
        
        auto imageLabel         = new PImageLabel("./assets/ui/title.png");
        auto imageViewText      = new PTextLabel("Image View");
        auto normalTextButton   = new PTextLabel("Normal Button");
        auto normalButton       = new PButton(351, 30, "I am normal button 1");
        auto unlimitedText      = new PTextLabel("Unlimited Text");
        auto limitedText        = new PTextLabel("Limited Text");
        auto unlimitedInput     = new PInput(198, 30, "Unlimited Input");
        auto limitedInput       = new PInput(198, 30, "Limited Input");
        auto stressButtonText   = new PTextLabel("Stress Button");
        auto stressButton       = new PStressButton(187, 36, "Hide the Icon");
        auto normalIconButton   = new PButton(187, 36, "Show The Icon");

        _inputText = new PTextLabel("(COLOR TEXT) Input context :");
        // Custom the text's color
        _inputText->FontColor = RGB(240, 56, 67);
        _inputText->Move(20, 328);

        _logo = new PImageLabel("./assets/ui/voidjang.png");
        _logo->Resize(47, 47);
        _logo->Move(333, 309);

        imageLabel->Resize(361, 53);
        
        unlimitedInput->Move(34, 198);
        unlimitedText->Move(34, 179);
        
        limitedInput->Move(227, 198);
        limitedText->Move(227, 179);
        
        normalTextButton->Move(34, 121);
        normalButton->Move(33, 144);
        
        imageLabel->Move(20, 58);
        imageViewText->Move(35, 35);
        
        stressButtonText->Move(22, 236);
        stressButton->Move(20, 260);
        
        normalIconButton->Move(227, 260);

        limitedInput->SetMaxLength(4);

        // Connect UI event, just like the Qt's signal, but little bit difference
        // on usage
        stressButton->OnClick.Connect(this, &UIExample::HideIcon);
        normalIconButton->OnClick.Connect(this, &UIExample::ShowIcon);
        limitedInput->OnFinish.Connect(this, &UIExample::OnLimitedInputChanged);
        limitedInput->OnText.Connect(this, &UIExample::OnLimitedInputChanged);

        // Add to UI manager
        manager.AddObject(imageLabel);
        manager.AddObject(imageViewText);
        manager.AddObject(normalTextButton); 
        manager.AddObject(normalButton);     
        manager.AddObject(unlimitedText);    
        manager.AddObject(limitedText);      
        manager.AddObject(unlimitedInput);   
        manager.AddObject(limitedInput);     
        manager.AddObject(stressButtonText); 
        manager.AddObject(stressButton);     
        manager.AddObject(normalIconButton);
        manager.AddObject(_logo);
        manager.AddObject(_inputText);

        setbkcolor(RGB(72, 73, 74));
        
        while (true)
        {
            ExMessage message;
            while (peekmessage(&message))
            {
                // Process message
                manager.OnMessage(message);
            }
            
            cleardevice();
            
            manager.OnDraw(device);
            device->Flush();
            
            Sleep(16);
        }
    }

private:
    void ShowIcon()
    {
        // Use show/hide function to show or hide a control
        _logo->Show();
    }
    void HideIcon()
    {
        _logo->Hide();
    }
    void OnLimitedInputChanged(PString Text)
    {
        // Adjust the text label size to adapt the text size
        _inputText->SetText(ostr::format("(COLOR TEXT) Input context :{}", Text));
        _inputText->ResizeAsText();
    }
    
private:
    PImageLabel *_logo;
    PTextLabel  *_inputText;
};

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    UIExample();
}
```

该程序已包含在 ```main.cpp``` 下，你可以通过定义 ```UI_DEMO``` 宏来运行它。

如果没有意外，那么你将看到下面的界面：

<div align="center">
    <img src="./README/UI-Demo.png">
</div>

该 UI 库是基于 Minecraft 基岩版的 UI 设计风格所制作的，当然也有不少的控件缺失。这可能会成为后面你绕不开的一环，特此我将向你深入介绍该 UI 库。

首先，看到 ```include/gui/GUIObject.h``` 内有一个名为 ```PGUIObject``` 的类，该类为所有控件的父类。其封装了许多控件的公用功能，例如 ```Resize```（用于设置控件大小），```Move```（用于控件排版）。你可以看到，```PGUIObject``` 有两个虚函数如下：
```C++
/// When the object was on drawing this function will be called
virtual void OnDraw() = 0;
/// When the object was on the message, this function
/// will be called
/// @param Message The message on called
/// @param Interface The interface of manager
/// @return If the control will take over this message, the return
/// should be true, nor false
virtual bool OnMessage(const ExMessage &Message, PGUIMangerInterface* Interface) { return false; }
```

其中 ``OnDraw`` 为纯虚函数，一定需要被实现，``OnMessage`` 默认返回 ``false``。

先说 ``OnDraw``，根据函数注释可知，该虚函数为当控件被 ``PManager`` 要求重绘时，该虚函数会被 GUI 管理器调用。我们以 ``PImageLabel`` 的实现为例：

```C++
void PImageLabel::OnDraw()
{
    HDC targetDC = GetImageHDC(GetWorkingImage());
    HDC sourceDC = _device->GetHDC();
    int width    = GetWidth();
    int height   = GetHeight();

    BLENDFUNCTION blendFunction = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    
    AlphaBlend(targetDC, _rectangle.left, _rectangle.top, width, height, sourceDC,
        0, 0, _image->GetWidth(), _image->GetHeight(), blendFunction);
}
```

通过这个实现我们可以清楚一点：在调用 OnDraw 前，GUI 管理器会自动设置好工作设备，无需手动 SetWorkingImage。

接下来是 ``OnMessage``，此处我们以 ``PButton`` 的实现为例：

```C++
bool PButton::OnMessage(const ExMessage &Message, PGUIMangerInterface* Interface)
{
    switch (Message.message)
    {
        case WM_MOUSEMOVE:
        {
            if (IsPointInside(Message.x, Message.y))
            {
                // 省略

                return true;
            }
            else
            {
                // 省略
            }
            
            return false;
        }
        case WM_LBUTTONDOWN:
        {
            if (IsPointInside(Message.x, Message.y))
            {
                // 省略
                Interface->RaiseAsFocus(this);
                
                return true;
            }
                
            return false;
        }
        case WM_LBUTTONUP:
        {
            if (Interface->GetFocusingObject() == this)
            {
                // 省略
                
                OnClick();

                auto instance = PGetSingleton<PAssetManager>().GetSoundInstance("click");

                if (instance)
                {
                    instance->Update();
                    PMCIManager::Play(instance, nullptr);
                }
                
                Interface->UnfocusObject(this);
            }
        }
        default:
        {
            return false;
        }
    }
}
```

首先关注 ``OnMessage`` 的返回值，可以发现默认返回的是 ``false``，其实就是 ``PGUIObject`` 的默认返回值。这里可以发现，如果返回值为 ``false`` 的话，那么该消息会被继续转递下去（也就是默认本控件不处理该消息）。否则该消息将会被在此截断，具体流程如下：

<div align="center">
    <img src="./README/UI-diagram.png">
</div>

这是一个非常简单的逻辑，却能保证控件在 `z` 轴上的次序正确，不会出现所谓 “`z fight` 现象”。

其中的 `IsPointInside` 函数涉及到了 `PGUIObject` 的一个属性：

```C++
protected:
    RECT _rectangle;
```

`_rectangle` 是控件的矩形包围盒（无论该控件视觉上是圆形还是异形的），`Resize` 和 `Move` 等方法都会直接修改这个属性。`IsPointInside` 会判断给定的点是否在 `_rectangle` 范围内，如果是，则返回 `true`，否则返回 `false`。

`PBUtton` 中涉及到的 `PGetSingleton<PAssetManager>()` 会在下文再介绍，这里请关注 `Interface->UnfocusObject(this)` 和 `Interface->RaiseAsFocus(this);` 方法。

这两个方法分别是将本控件提升/取消为焦点控件，在 GUI 管理器中，作为焦点控件的控件在消息方面会有特殊处理。这里关注 `include/gui/GUIMangaer.h`中 `GUIManager` 的 `OnMessage` 方法，这个方法的实现如下：

```C++
void PGUIManager::OnMessage(ExMessage &Message)
{
    if (_focus)
    {
        if (!_focus->OnMessage(Message, this))
        {
            for (auto &object : _child)
            {
                if (!object->IsVisible())
                {
                    continue;
                }
                if (object != _focus && object->OnMessage(Message, this))
                {
                    break;
                }
            }
        }
        else
        {
            return;
        }
    }
    for (auto &object : _child)
    {
        if (!object->IsVisible())
        {
            continue;
        }
        if (object->OnMessage(Message, this))
        {
            break;
        }
    }
}
```

可以看到，`if (_focus)` 判断是否存在焦点控件，如果存在焦点控件则会优先将消息交由焦点控件处理，换而言之，事件处理逻辑变成了下面的流程：

<div align="center">
    <img src="./README/UI-focus-diagram.png">
</div>


再注意到 `PGUIObject` 下还有一个默认为空的虚函数如下：

```cpp
public:
    /// When the object's focus was taken over by other object, the rest function will be called
    virtual void Reset()
    {
        
    }
```

根据函数注释可知，当前函数控件**被动失去焦点**时，该函数会被调用以复位控件状态。

基于上面的描述，相信你已经可以自己创建一个自己的控件并重写父类的 `OnDraw` 方法和 `OnMessage` 方法了。若还有不清楚的地方一定记住**查看已有代码+查看代码注释**。

5. 单例模式实现：在本项目中，很多类（例如 `PAssetManager`）需要通过单例模式实现，而我在代码中提供了一个单例模式的编译时模板实现，请参考 `include/singleton/Singleton.h`。

整个头文件中只有简短的十二行代码：
```cpp
///
/// The helper class for singleton pattern in paper library
///

#pragma once

template<class Type>
Type& PGetSingleton()
{
    static Type instance;
    return instance;
}
```

而当你需要调用某一类型的单例时，只需使用类似：`auto& object = PGetSingleton<Type>();` 即可。但请注意，在你要创建一个类型的单例时，**请一定确保 `PGetSingleton<Type>()` 在被参与编译的某个源码文件中调用了！**这非常重要，因为只有这样编译器才会生成对应类型的单例。

6. 资源管理器：资源管理器 `PAssetManager` 需要通过单例器（`PGetSingleton<PAssetManager>();`） 调用，请参考 `include/assets/AssetManager.h`。

请注意，`PAssetManager` 默认只会初始化贴图资源，而并不会加载 *assets/sound* 目录下的音频到 MCI 管理器中。这么做是因为加载音频需要一定的时间，因此我将加载音频的逻辑放在了游戏加载画面中。请参考 `source/game/Game.cpp` 中的 `PLGame::Loop` 方法：

```Cpp
bool musicLoadDone = false;
std::thread([&musicLoadDone]()
{
    PGetSingleton<PAssetManager>().LoadSounds();
    musicLoadDone = true;
}).detach();

float progress = 0.0;

while (_progressBar->GetPercentage() < 100)
{
    ExMessage message;
    while (peekmessage(&message))
    {
        _manager->OnMessage(message);
    }
        
    cleardevice();

    _progressBar->SetPercentage(SmoothInterpolation(progress));
        
    _manager->OnDraw(_windowDevice);
    
    _windowDevice->Flush();
    // We are actually loading resource of music :)
    if ((progress >= 70 && musicLoadDone) || progress < 70)
    {
    progress += 0.2;
    }
        
    Sleep(16);
}
```

可以看到，音频文件的加载是通过 `std::thread` 调用 `PAssetManager::LoadSounds` 异步执行的，且若在进度条达到 70% 前音频文件依然加载完成，则进度条会被卡死在 70% 处。若你也有其他的加载需要且需要通过游戏加载画面展示，请参考如上写法，或自己编写一个 `PProgressController`。

最后有关相关 API 的用法以及参数不再赘述，请参考源码中的注释。

## 你可能要接着做？

你可能要完善 `(include&source)/game/component` 下的组件系统以及 `(include&source)/game/Block.h`（对我的世界方块的抽象定义基类，并抽象出 `PLiquidBlock`、`PPlaneBlock` 等变体）。

然后就是你的自由发挥时间了，当然你不想按着我的走也可以，直接把我的 TODO 代码全部删除即可。