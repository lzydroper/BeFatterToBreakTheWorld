# 肥破苍穹

基于 **C++ / SFML 2** 的 2D 俯视角动作刷怪 RPG（Visual Studio 2022 工程）。玩家在瓦片地图上移动、挥剑击杀刷怪点刷出的怪物获取经验；升级时随机抽取三选一属性成长，**角色升到 10 级即获胜**，HP 归零则失败。

> 课程作品：代码中混有拼音/英文命名、调试残留与早期草稿（见 `ToDo.txt`），仅供学习参考。

## 运行

### 直接运行
构建产物与运行所需文件已打包在输出目录，进入对应目录双击即可（SFML DLL、字体、资源、地图、着色器均已就位）：

```
x64\Debug\肥破苍穹.exe
x64\Release\肥破苍穹.exe
```

### 从源码构建
1. 安装 Visual Studio 2022（“使用 C++ 的桌面开发”，MSVC v143）。
2. 打开 `肥破苍穹.sln`（仓库根部与工程目录内各有一份，引用同一工程），配置选 **x64** + Debug/Release 并生成。
3. SFML 头文件与导入库已随仓库提供（`肥破苍穹/include`、`肥破苍穹/lib`），无需另行安装。
4. 游戏按 **相对当前工作目录** 加载资源（`Config/`、`Fonts/`、`Resources/`、`map.slmp`、两个 shader、DLL）。直接运行请使用输出目录里的副本；若在 VS 内调试，可将工作目录设为 exe 所在目录。

## 玩法与操作

| 场景 | 操作 |
|---|---|
| 主菜单 | `New Game` 新游戏 / `Editor` 地图编辑器 / `Settings` 设置 / `Quit` 退出 |
| 游戏内 | `WASD` 移动；鼠标控制朝向；按住**鼠标左键**挥剑攻击；`Tab` 开关角色信息面板；`Esc` 暂停（菜单：`Settings` / `Quit`，再按 `Esc` 返回） |
| 升级 | 从「生命、力量、敏捷、防御、幸运、剑长」六种奖励中随机抽取 3 项，三选一 |
| 调试 | 游戏内按 `Enter` 进入作弊控制台，在控制台窗口按编号修改等级/HP/属性/经验 |

键位均可改：见 `Config/supported_keys.ini` 与各状态 `Config/*_keybinds.ini`。

## 内部构造

### 1. 总架构：窗口 + 状态栈
入口 `main.cpp` 只构造 `Game` 并调用 `run()`。`Game` 负责：

- 读 `Config/graphics.ini` 创建 `sf::RenderWindow`（分辨率/全屏/帧率/垂直同步/抗锯齿/音量）；
- 读 `Config/supported_keys.ini` 建立“键名→键值”映射，各状态再按自己的 `*_keybinds.ini` 绑定动作；
- 持有 `std::stack<State*>` 状态栈和共享上下文 `StateData`（窗口、图形设置、键表、状态栈、音频管理器、gridSize=64）；
- 主循环：计算帧时间 `dt` → 更新并渲染**栈顶**状态 → 栈空退出。

`State` 是所有场景的基类，内置四套鼠标坐标换算（屏幕/窗口/视图/网格）、按键节流计时、暂停/退出接口。内存为手动管理：状态入栈即 `new`，退出时由 `Game` 手动 `delete`。

### 2. 状态机（场景）
```
MainMenuState ── 主菜单
   ├─ GameState     主游戏玩法
   ├─ EditorState   瓦片地图编辑器
   └─ SettingsState 图形/音量设置（可压栈到任意状态之上，如暂停菜单里的 Settings）
```
- `GameState` 内部再用枚举细分 `GAME / PAUSE / LEVELUP / GAMEOVER` 四个子状态，分别对应 `PauseMenu`、`LevelUpPause`、`GameOverPause` 三个覆盖层。
- `GameState` 用 `sf::RenderTexture` 离屏渲染：先以跟随玩家的 `sf::View` 绘制世界（瓦片 → 敌人 → 玩家 → 前景装饰），切回默认视图再叠加 HUD。场景精灵统一经过自定义着色器（`vertex_shader.vert` / `fragment_shader.frag`），以玩家位置为光源做径向光照。
- 飘字由 `TextTagSystem` 统一管理：按模板（`DEFAULT/NEGATIVE/POSITIVE/EXPERIENCE/DEBUFF`）生成伤害、经验、状态提示，自动上浮/淡出/过期回收。

### 3. 实体与组件
`Entity` 是玩家与敌人的公共基类，采用组合方式挂载五个组件（均可为空，由 `createXxxComponent()` 按需创建）：

| 组件 | 职责 |
|---|---|
| `HitboxComponent` | 相对精灵偏移的受击盒 |
| `MovementComponent` | 带加速度/减速度/最大速度的移动，输出 `IDLE / MOVING_*` 状态 |
| `AnimationComponent` | 精灵图集逐帧动画：注册多段动画、优先级动画、随速度变速 |
| `AttributeComponent` | 成长数值：等级/经验与升级公式、力量/敏捷/防御/幸运/剑长、HP、伤害区间；`updateStats()` 按等级重算（玩家与怪物两套公式），`reset` 标志触发满血刷新 |
| `SkillComponent` | 技能经验框架，目前仅雏形，玩法中未使用 |

- **玩家 `Player`**：WASD 移动；鼠标定朝向；出生即装备 1 级 `Sword`，按住左键挥砍（伤害 = 属性+武器伤害区间随机，按幸运概率触发双倍并吸血）；受击存在约 0.5 s 无敌帧；击杀得经验（飘字+音效）；中毒 `Poisoning`、减速 `Retarding` 两个减益对象直接挂在玩家上，由 `GameState` 每帧结算。
- **敌人 `Enemy`**（抽象基类）及五个实现：`Rat`（鼠）、`Bird1`（鸟）、`Spider`（蜘蛛）、`Scorpion`（蝎）、`Slime`（史莱姆，`SMALL~SUPERHUGE` 体型影响属性）。由刷怪点创建后经 `AIFollow` 追逐玩家，接触造成「敌伤害 − 玩家防御（最小 1）」；蜘蛛附加中毒、史莱姆附加减速；离开玩家约 1500 px 之外开始计时消失；死亡掉落经验。`EnemySystem` 统一负责敌人的创建与移除。

### 4. 地图与关卡
- `TileMap` 以多维 vector 存瓦片（`map[x][y][层][同格多个瓦片]`），支持：
  - **纯文本地图存取**（`.slmp`）：头部依次为 网格宽高、gridSize、层数、瓦片纹理路径，之后逐格记录每个瓦片；`EnemySpawnerTile` 额外带怪物类型/数量/刷怪冷却/最大距离；
  - 世界边界与瓦片碰撞（用实体“下一帧位置”求交并回弹）；
  - **视口裁剪**：只对玩家周围一小圈格子执行更新与渲染；`DOODAD` 装饰瓦片压入 `deferredRenderStack` 延迟到角色之后绘制，形成遮挡关系。
- 瓦片类型 `TileTypes`：`DEFAULT / DAMAGING / DOODAD / ENEMYSPAWNER`。基类 `Tile` 派生 `RegularTile`（普通/装饰）与 `EnemySpawnerTile`（**刷怪点**：玩家靠近时按冷却补怪，直到在场数量达上限；敌人死亡回调递减计数）。
- 内置关卡 `map.slmp`：100×100 格、每格 64 px、10 层，瓦片纹理 `Resources/Images/Tiles/tilesheet3.png`。

**地图编辑器 `EditorState`**（主菜单 `Editor` 进入）：`WASD` 移动镜头、`PageUp/PageDown` 切换两种模式——

- `DefaultEditorMode`：从 `TextureSelector` 选图块，左键放置 / 右键删除；`Tab` 切换该瓦片是否碰撞、`↑/↓` 切换瓦片类型、`L` 锁定放置；
- `EnemyEditorMode`：放置/删除刷怪瓦片，`Num1~Num4`（Shift 同时按下为减少）调整怪物类型、数量、刷怪冷却、最大距离；
- `Esc` 打开暂停菜单：`SAVE` / `LOAD` 写回或重载 `map.slmp`，`QUIT` 返回主菜单。

### 5. 装备与背包
- 物品继承链 `Item → Weapon（伤害区间/攻击距离/攻击节流）→ MeleeWeapon → Sword`，目前唯一武器为 `Sword`。攻击判定：剑的 `weapon_sprite` 包围盒与敌人相交。
- `Inventory` 是定容 `Item**` 数组骨架（含增删与文件存取接口），尚未接入掉落/拾取玩法。

### 6. UI / GUI
- `gui` 命名空间提供分辨率无关布局（`p2pX/p2pY` 百分比、`calcCharSize`）与控件：`Button`、`DropDownList`（设置里的分辨率/音量）、`TextureSelector`（编辑器选贴图）、`ProgressBar`。
- `PlayerGUI` 即 HUD：等级文字、经验条、HP 条；`PlayerGUITabs`/`Tab` 是面板抽象，`CharacterTab` 以文本展示属性、武器数值与当前减益。

### 7. 音频
- `AudioManager` 统筹两类播放器：`BackgroundMusic`（BGM，`sf::Music` 多轨缓存，支持循环/暂停/音量）与 `SoundEffect`（短音效，`sf::SoundBuffer` + `sf::Sound`）。
- 各状态切换音乐由状态代码点播（主菜单/战斗/暂停/胜利/失败 BGM，攻击/受伤/升级等音效），音量统一取 `GraphicsSettings.volumelevel`。

### 8. 其它
- `VectorMath`：`vectorDistance` 等少量数学工具。
- `stdafx.h`：统一包含头；其中 `#define test false` 是编译期调试开关（测试输出、碰撞盒渲染、调试文本）。
- `GraphicsSettings` 与 `Config/graphics.ini`：标题、分辨率、全屏、帧率上限、垂直同步、抗锯齿、音量（设置界面可改并写回）。

## 目录结构

```
Crapworks (1)/
├─ 肥破苍穹.sln                 # 解决方案（工程目录内另有一份等价 .sln）
├─ 肥破苍穹/                    # 工程目录 = $(ProjectDir)，源码均在此层
│  ├─ 肥破苍穹.vcxproj          # 工程文件
│  ├─ *.cpp / *.h               # 全部源码（含统一包含头 stdafx.h）
│  ├─ include/  lib/            # SFML 头文件 / 导入库
│  ├─ bin/                      # 运行时 DLL（sfml-*.dll、openal32.dll）
│  ├─ Config/                   # graphics.ini、supported_keys.ini、各状态键位
│  ├─ Fonts/  Resources/        # 字体；背景图/角色与敌人精灵/瓦片图集/音乐/音效
│  ├─ map.slmp                  # 内置地图（纯文本格式）
│  ├─ vertex_shader.vert / fragment_shader.frag   # 光照着色器
│  ├─ ToDo.txt                  # 开发待办与备注
│  └─ 肥破苍穹/                 # 早期原型副本 + 原始美术素材（不参与本工程构建）
└─ x64/Debug|Release/           # 构建输出：exe + DLL + Config/Fonts/Resources/map/shader 副本
```

## 已知情况
- `ToDo.txt` 记录的未完成项：怪物掉落/拾取、粒子系统、玩家存档等；`SkillComponent`、`Inventory` 目前只有骨架。
- `肥破苍穹/肥破苍穹/` 是早期原型与下载素材的备份，包含一份不被本解决方案引用的旧工程，可忽略或清理。
- 仓库内含构建产物与大量美术素材，体积较大；编译时开启 `SDLCheck` 且个别文件带有中文乱码注释，不影响构建。
