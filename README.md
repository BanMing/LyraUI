# LyraUI

该项目将`Lyra`中UI模块提取出独立成插件，以便于添加到不同项目中。这篇文档主要讲解如何将该插件接入项目以及一些说明。

## Setup

打开插件：

![](./.github/2025-08-20-20-54-43.png)

`Project Setting`设置：

- ![](./.github/2025-11-28-02-22-50.png)
- ![](./.github/2025-11-28-02-49-08.png)
- ![](./.github/2025-11-28-02-49-51.png)
- 需要配置输入和默认UIPolicy,具体配置在下文中。

测试场景：/All/Plugins/GameCommonUIBase/MainMenu

## Widget

![](./.github/2025-11-27-23-57-46.png)

GameUIMessagingSubsystem管理GameUIPolicy，GameUIPolicy决定使用什么样的PrimaryGameLayout，接着PrimaryGameLayout会有多个Layer，每个Layer是一个Switcher，用于控制当前Layer下面显示什么Panel。

### GameUIManagerSubsystem
GameUIManager的主要作用就是负责UIPolicy的加载与切换,同时在玩家加入游戏时（本地多人）可以调用NotifyPlayerAdded等函数执行一些操作，并且调用UIPolicy中对应的方法。

同时每帧都会设置所有玩家的PrimaryGameLayout和HUD的可见性同步。

### GameUIPolicy

GameUIPolicy的工作，一言以蔽之就是根据本地玩家的状态（添加、删除或销毁）和本地多人交互的模式（单人、轮换和多人），来决定将RootLayout向视口加入或删除。并且向外提供一些get函数。 另外需要注意UCLASS宏内的Within = GameUIManagerSubsystem说明GameUIPolicy只能由GameUIManagerSubsystem的实例创建，不能单独创建。

需要在Project Setting中配置：

![](./.github/2025-11-28-01-25-47.png)

### PrimaryGameLayout

PrimaryGameLayout负责Layer的注册，以及Widget到Layer的push和remove。此外还有布尔类型的值bIsDormant和相关函数，用于给GameUIPolicy判断本地玩家的RootLayout是否显示在屏幕上。

PrimaryGameLayout就是一个UCommonUserWidget。

当前默认的布局如下：

![](./.github/2025-11-28-01-27-07.png)


- GameLayer是游戏内UI,类似于HUD,通常用于显示游戏状态，即生命条、武器槽等。
- GameMenu是游戏内的菜单，即背包等。
- Menu是游戏菜单，例如游戏开始界面、设置界面等。
- Modal是游戏弹窗,例如保存游戏或者更改游戏设置时弹窗的确定弹窗之类。

每个层就是一个UCommonActivatableWidgetStack，内部包含了一个Switcher，将Panel添加到Layer时，会将Panel添加到Switcher里，再通过设置switcher的index来切换Panel。

每一个Layer对应着一个GameplayTag：

![](./.github/2025-11-28-01-35-03.png)

将Panel加到Layer中使用：

![](./.github/2025-11-28-01-37-23.png)

将Panel移除Layer使用：

![](./.github/2025-11-28-01-38-15.png)

### Split Screen

Lyra中为了实现分屏UI，设计了GameUIPolicy和PrimaryGameLayout。这里可以简单理解：
- 在多人游戏且在同一台机器上运行时，UI的规则是GameUIPolicy A，每一个玩家有一个PrimaryGameLayout A，也就是UI布局。
- 在单人游戏时，UI的规则GameUIPolicy B。现在的玩家的UI布局就是PrimaryGameLayout B

这样有些抽象，可以看下图：

![](./.github/2025-11-28-00-54-05.png)

由此可以知道一个玩家对应自己UI布局，在项目中使用LocalPlayer来定义的玩家信息。

根据不同玩家加载不同的UI布局触发流程：
- UGameUIGameInstance::AddLocalPlayer 有本地玩家加入到游戏
- UGameUIManagerSubsystem::NotifyPlayerAdded
- UGameUIPolicy::NotifyPlayerAdded 监听本地玩家控制器设置
- AGameUIPlayerController::ReceivedPlayer 本地玩家控器设置完成
- UGameUILocalPlayer::OnPlayerControllerSet 触发委托
- UGameUIPolicy::CreateLayoutWidget 创建布局UI（PrimaryGameLayout）
- UGameUIPolicy::AddLayoutToViewport 添加布局UI到视窗

所以需要需要将以下文件中的实现复制到项目中对应的文件中去：
- GameUILocalPlayer
- GameUIGameInstance
- GameUIPlayerController

同时需要在Project Setting和GameMode中设置对应。

值得注意的是如果游戏不需要分屏UI这种功能时，GameUIPolicy是可以不需要的，在游戏开始时直接将PrimaryGameLayout加载的视窗渲染即可。

### Panel

![](./.github/2025-11-28-02-13-16.png)

因为Panel都是有Switcher管理的，所以所有的Panel都需要继承UGameUIActivatablePanel，也就是需要继承至UCommonActivatableWidget。这样才能有Switcher控制显示隐藏。

必须实现GetDesiredFocusTarget方法，用于激活面板时聚焦那个Widget。

#### Slot Panel
其中有一个特殊的Panel - GameUIHUDLayoutPanel，用于制作游戏中的HUD也就是战斗中UI。特殊点是它使用GameUISlotPointWidget对子Widget进行占位，使用GameplayTag对应子Widget，动态加载不同的子Widget。

这样做到子Widget和Panel之间的完全解耦。

其中WBP_GameHUDLayout如下图：

![](./.github/2025-11-28-01-56-55.png)

在GameUISlotPointWidget对应配置：

![](./.github/2025-11-28-01-58-02.png)

加载GameplayTag对应的widget和卸载：

![](./.github/2025-11-28-01-59-46.png)

子Widget:

![](./.github/2025-11-28-02-17-56.png)

最终显示效果：

![](./.github/2025-11-28-02-15-56.png)


其原理是在GameUISlotPointWidget在构造时，会去执行RebuildWidget方法，在该方法把SlotPointTag注册到GameUISlotSubSystem中，然后存储起来。

当调用GameUISlotSubSystem::RegisterSlot时，也就是上图。会去查询SlotPointTag是否有GameUISlotPointWidget注册过。如果有就将子widget创建出来。

值得注意的是，如果没有需要Panel中布局是一定的，内容会根据情况不同的需求时。直接使用完整的panel，做会更加只管一些，不一定非要用这中方式。

#### Message

![](./.github/2025-11-28-02-14-05.png)

调用方式：

![](./.github/2025-11-28-02-24-56.png)

也可以直接调用：
- UGameUIMessagingSubsystem::ShowConfirmation
- UGameUIMessagingSubsystem::ShowError

需要在Project Setting中配置默认的Panel：

![](./.github/2025-11-28-02-28-04.png)

### Widget

在Widget目录下有一些复写的基础控件

#### InputActionWidget

一般会配合着按键使用，作为子Widget放在按键的Widget蓝图中。如果配置Input Actions也就是下图：

![](./.github/2025-11-28-02-34-55.png)

意味着指定按键对应该触发点击事件。如果不指定就是默认按键触发点击事件。

#### Button

基础按键结构如下：

![](./.github/2025-11-28-02-31-29.png)

#### Tab

![](./.github/2025-11-28-02-14-45.png)

设置选项卡配置：

![](./.github/2025-11-28-02-41-14.png)

每一个选项都要可以设置：
- 选项按钮
- 选项内容Widget

需要将内容控制的Switcher注册到选项列表组件中：

![](./.github/2025-11-28-02-43-19.png)

## Input

在Common UI中一定不要使用任何关于Enhanced Input相关的设置，Common UI对Enhanced Input的适配并没有非常的完善。如果Gameplay中使用了Enhanced Input，也会跟UI的输入打架。所以这里就是使用Common Input即可。

### Setup
- 首先新建一个输入数据表<br>![](./.github/2025-11-28-02-51-47.png)
- 在该表中新建4个按键行为<br>![](./.github/2025-11-28-02-52-53.png)
- 接着创建InputData<br>![](./.github/2025-11-28-02-53-45.png)
- 默认有设置对应的InputActionTable，这个表中几乎有所有按键的设置，如果需要可以直接复制其中的选项到我们新建的表即可。![](./.github/2025-11-28-02-55-16.png)
- 在Project Setting中就可以设置输入数据 <br> ![](./.github/2025-11-28-02-55-55.png)
- 接着创建Controller Data<br>![](./.github/2025-11-28-02-57-46.png)
- 该数据是设置不同平台的不同设配的控制数据，这里PC就可以创建两个<br>![](./.github/2025-11-28-02-58-19.png)
- 设置按键与图片<br>![](./.github/2025-11-28-02-59-07.png)
- 在设置Gamepad Name时，需要和Project Setting中对应平台的Default Gamepad Name一致<br>![](./.github/2025-11-28-03-00-07.png)

### Display Name

![](./.github/2025-11-28-03-01-07.png)

在按键上会被设置：

![](./.github/2025-11-28-03-01-51.png)