cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 8.1 CACHE STRING "" FORCE)

project(MapleStory CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Set environment variables
################################################################################
set(ENV{PlatformTarget} ${CMAKE_VS_PLATFORM_NAME})
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ENV{PlatformArchitecture} "64")
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ENV{PlatformArchitecture} "86")
endif()

################################################################################
# Sub-projects
################################################################################


























set(PROJECT_NAME MapleStory)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Audio/Audio.h"
    "Character/ActiveBuffs.h"
    "Character/Buff.h"
    "Character/Char.h"
    "Character/CharEffect.h"
    "Character/CharStats.h"
    "Character/EquipStat.h"
    "Character/ExpTable.h"
    "Character/Inventory/Equip.h"
    "Character/Inventory/EquipQuality.h"
    "Character/Inventory/Inventory.h"
    "Character/Inventory/InventoryType.h"
    "Character/Inventory/Item.h"
    "Character/Inventory/Pet.h"
    "Character/Inventory/Weapon.h"
    "Character/Job.h"
    "Character/Look/Afterimage.h"
    "Character/Look/Body.h"
    "Character/Look/BodyDrawInfo.h"
    "Character/Look/CharEquips.h"
    "Character/Look/CharLook.h"
    "Character/Look/Clothing.h"
    "Character/Look/EquipSlot.h"
    "Character/Look/Face.h"
    "Character/Look/Hair.h"
    "Character/Look/PetLook.h"
    "Character/Look/Stance.h"
    "Character/MapleStat.h"
    "Character/MonsterBook.h"
    "Character/OtherChar.h"
    "Character/PassiveBuffs.h"
    "Character/Player.h"
    "Character/PlayerStates.h"
    "Character/QuestLog.h"
    "Character/SkillBook.h"
    "Character/SkillId.h"
    "Character/StatCaps.h"
    "Character/TeleportRock.h"
    "Configuration.h"
    "Constants.h"
    "Data/BulletData.h"
    "Data/EquipData.h"
    "Data/ItemData.h"
    "Data/JobData.h"
    "Data/SkillData.h"
    "Data/WeaponData.h"
    "Error.h"
    "Gameplay/Camera.h"
    "Gameplay/Combat/Attack.h"
    "Gameplay/Combat/Bullet.h"
    "Gameplay/Combat/Combat.h"
    "Gameplay/Combat/DamageNumber.h"
    "Gameplay/Combat/RegularAttack.h"
    "Gameplay/Combat/Skill.h"
    "Gameplay/Combat/SkillAction.h"
    "Gameplay/Combat/SkillBullet.h"
    "Gameplay/Combat/SkillHitEffect.h"
    "Gameplay/Combat/SkillSound.h"
    "Gameplay/Combat/SkillUseEffect.h"
    "Gameplay/Combat/SpecialMove.h"
    "Gameplay/MapleMap/Drop.h"
    "Gameplay/MapleMap/ItemDrop.h"
    "Gameplay/MapleMap/Layer.h"
    "Gameplay/MapleMap/MapBackgrounds.h"
    "Gameplay/MapleMap/MapChars.h"
    "Gameplay/MapleMap/MapDrops.h"
    "Gameplay/MapleMap/MapEffect.h"
    "Gameplay/MapleMap/MapInfo.h"
    "Gameplay/MapleMap/MapMobs.h"
    "Gameplay/MapleMap/MapNpcs.h"
    "Gameplay/MapleMap/MapObject.h"
    "Gameplay/MapleMap/MapObjects.h"
    "Gameplay/MapleMap/MapPortals.h"
    "Gameplay/MapleMap/MapReactors.h"
    "Gameplay/MapleMap/MapTilesObjs.h"
    "Gameplay/MapleMap/MesoDrop.h"
    "Gameplay/MapleMap/Mob.h"
    "Gameplay/MapleMap/Npc.h"
    "Gameplay/MapleMap/Obj.h"
    "Gameplay/MapleMap/Portal.h"
    "Gameplay/MapleMap/Reactor.h"
    "Gameplay/MapleMap/Tile.h"
    "Gameplay/Movement.h"
    "Gameplay/Physics/Foothold.h"
    "Gameplay/Physics/FootholdTree.h"
    "Gameplay/Physics/Physics.h"
    "Gameplay/Physics/PhysicsObject.h"
    "Gameplay/Playable.h"
    "Gameplay/Spawn.h"
    "Gameplay/Stage.h"
    "Graphics/Animation.h"
    "Graphics/Color.h"
    "Graphics/DrawArgument.h"
    "Graphics/EffectLayer.h"
    "Graphics/Geometry.h"
    "Graphics/GraphicsGL.h"
    "Graphics/SpecialText.h"
    "Graphics/Sprite.h"
    "Graphics/Text.h"
    "Graphics/Texture.h"
    "IO/Components/AreaButton.h"
    "IO/Components/Button.h"
    "IO/Components/Charset.h"
    "IO/Components/ChatBalloon.h"
    "IO/Components/EquipTooltip.h"
    "IO/Components/Gauge.h"
    "IO/Components/Icon.h"
    "IO/Components/IconCover.h"
    "IO/Components/ItemTooltip.h"
    "IO/Components/MapleButton.h"
    "IO/Components/MapleComboBox.h"
    "IO/Components/MapleFrame.h"
    "IO/Components/MapTooltip.h"
    "IO/Components/NameTag.h"
    "IO/Components/ScrollingNotice.h"
    "IO/Components/SkillTooltip.h"
    "IO/Components/Slider.h"
    "IO/Components/StatefulIcon.h"
    "IO/Components/Textfield.h"
    "IO/Components/TextTooltip.h"
    "IO/Components/Tooltip.h"
    "IO/Components/TwoSpriteButton.h"
    "IO/Cursor.h"
    "IO/KeyAction.h"
    "IO/Keyboard.h"
    "IO/KeyConfig.h"
    "IO/KeyType.h"
    "IO/Messages.h"
    "IO/UI.h"
    "IO/UIDragElement.h"
    "IO/UIElement.h"
    "IO/UIState.h"
    "IO/UIStateCashShop.h"
    "IO/UIStateGame.h"
    "IO/UIStateLogin.h"
    "IO/UITypes/UIAranCreation.h"
    "IO/UITypes/UIBuffList.h"
    "IO/UITypes/UICashShop.h"
    "IO/UITypes/UIChannel.h"
    "IO/UITypes/UICharInfo.h"
    "IO/UITypes/UICharSelect.h"
    "IO/UITypes/UIChat.h"
    "IO/UITypes/UIChatBar.h"
    "IO/UITypes/UICommonCreation.h"
    "IO/UITypes/UICygnusCreation.h"
    "IO/UITypes/UIEquipInventory.h"
    "IO/UITypes/UIEvent.h"
    "IO/UITypes/UIExplorerCreation.h"
    "IO/UITypes/UIGender.h"
    "IO/UITypes/UIItemInventory.h"
    "IO/UITypes/UIJoypad.h"
    "IO/UITypes/UIKeyConfig.h"
    "IO/UITypes/UILogin.h"
    "IO/UITypes/UILoginNotice.h"
    "IO/UITypes/UILoginWait.h"
    "IO/UITypes/UILogo.h"
    "IO/UITypes/UIMiniMap.h"
    "IO/UITypes/UINotice.h"
    "IO/UITypes/UINpcTalk.h"
    "IO/UITypes/UIOptionMenu.h"
    "IO/UITypes/UIQuestLog.h"
    "IO/UITypes/UIQuit.h"
    "IO/UITypes/UIRaceSelect.h"
    "IO/UITypes/UIRegion.h"
    "IO/UITypes/UIShop.h"
    "IO/UITypes/UISkillBook.h"
    "IO/UITypes/UISoftKey.h"
    "IO/UITypes/UIStatsInfo.h"
    "IO/UITypes/UIStatusBar.h"
    "IO/UITypes/UIStatusMessenger.h"
    "IO/UITypes/UITermsOfService.h"
    "IO/UITypes/UIUserList.h"
    "IO/UITypes/UIWorldMap.h"
    "IO/UITypes/UIWorldSelect.h"
    "IO/UITypes/UIWorldSelectBackground.h"
    "IO/Window.h"
    "MapleStory.h"
    "Net/Cryptography.h"
    "Net/Handlers/AttackHandlers.h"
    "Net/Handlers/CashShopHandlers.h"
    "Net/Handlers/CommonHandlers.h"
    "Net/Handlers/Helpers/CashShopParser.h"
    "Net/Handlers/Helpers/CharacterParser.h"
    "Net/Handlers/Helpers/ItemParser.h"
    "Net/Handlers/Helpers/LoginParser.h"
    "Net/Handlers/Helpers/MovementParser.h"
    "Net/Handlers/InventoryHandlers.h"
    "Net/Handlers/LoginHandlers.h"
    "Net/Handlers/MapObjectHandlers.h"
    "Net/Handlers/MessagingHandlers.h"
    "Net/Handlers/NpcInteractionHandlers.h"
    "Net/Handlers/PlayerHandlers.h"
    "Net/Handlers/PlayerInteractionHandlers.h"
    "Net/Handlers/SetFieldHandlers.h"
    "Net/Handlers/TestingHandlers.h"
    "Net/InPacket.h"
    "Net/Login.h"
    "Net/NetConstants.h"
    "Net/OutPacket.h"
    "Net/PacketError.h"
    "Net/PacketHandler.h"
    "Net/Packets/AttackAndSkillPackets.h"
    "Net/Packets/CharCreationPackets.h"
    "Net/Packets/CommonPackets.h"
    "Net/Packets/CustomOutPacket.h"
    "Net/Packets/GameplayPackets.h"
    "Net/Packets/InventoryPackets.h"
    "Net/Packets/LoginPackets.h"
    "Net/Packets/MessagingPackets.h"
    "Net/Packets/MovementPacket.h"
    "Net/Packets/NpcInteractionPackets.h"
    "Net/Packets/PlayerInteractionPackets.h"
    "Net/Packets/PlayerPackets.h"
    "Net/Packets/SelectCharPackets.h"
    "Net/PacketSwitch.h"
    "Net/Session.h"
    "Net/SocketAsio.h"
    "Net/SocketWinsock.h"
    "resource.h"
    "Template/BoolPair.h"
    "Template/Cache.h"
    "Template/Enumeration.h"
    "Template/EnumMap.h"
    "Template/Interpolated.h"
    "Template/Optional.h"
    "Template/Point.h"
    "Template/Range.h"
    "Template/Rectangle.h"
    "Template/Singleton.h"
    "Template/TimedQueue.h"
    "Template/TypeMap.h"
    "Timer.h"
    "Util/HardwareInfo.h"
    "Util/Lerp.h"
    "Util/Misc.h"
    "Util/NxFiles.h"
    "Util/QuadTree.h"
    "Util/Randomizer.h"
    "Util/ScreenResolution.h"
    "Util/TimedBool.h"
    "Util/WzFiles.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Audio/Audio.cpp"
    "Character/ActiveBuffs.cpp"
    "Character/Buff.cpp"
    "Character/Char.cpp"
    "Character/CharEffect.cpp"
    "Character/CharStats.cpp"
    "Character/EquipStat.cpp"
    "Character/ExpTable.cpp"
    "Character/Inventory/Equip.cpp"
    "Character/Inventory/EquipQuality.cpp"
    "Character/Inventory/Inventory.cpp"
    "Character/Inventory/InventoryType.cpp"
    "Character/Inventory/Item.cpp"
    "Character/Inventory/Pet.cpp"
    "Character/Inventory/Weapon.cpp"
    "Character/Job.cpp"
    "Character/Look/Afterimage.cpp"
    "Character/Look/Body.cpp"
    "Character/Look/BodyDrawInfo.cpp"
    "Character/Look/CharEquips.cpp"
    "Character/Look/CharLook.cpp"
    "Character/Look/Clothing.cpp"
    "Character/Look/EquipSlot.cpp"
    "Character/Look/Face.cpp"
    "Character/Look/Hair.cpp"
    "Character/Look/PetLook.cpp"
    "Character/Look/Stance.cpp"
    "Character/MapleStat.cpp"
    "Character/MonsterBook.cpp"
    "Character/OtherChar.cpp"
    "Character/PassiveBuffs.cpp"
    "Character/Player.cpp"
    "Character/PlayerStates.cpp"
    "Character/QuestLog.cpp"
    "Character/SkillBook.cpp"
    "Character/TeleportRock.cpp"
    "Configuration.cpp"
    "Data/BulletData.cpp"
    "Data/EquipData.cpp"
    "Data/ItemData.cpp"
    "Data/JobData.cpp"
    "Data/SkillData.cpp"
    "Data/WeaponData.cpp"
    "Gameplay/Camera.cpp"
    "Gameplay/Combat/Bullet.cpp"
    "Gameplay/Combat/Combat.cpp"
    "Gameplay/Combat/DamageNumber.cpp"
    "Gameplay/Combat/RegularAttack.cpp"
    "Gameplay/Combat/Skill.cpp"
    "Gameplay/Combat/SkillAction.cpp"
    "Gameplay/Combat/SkillBullet.cpp"
    "Gameplay/Combat/SkillHitEffect.cpp"
    "Gameplay/Combat/SkillSound.cpp"
    "Gameplay/Combat/SkillUseEffect.cpp"
    "Gameplay/MapleMap/Drop.cpp"
    "Gameplay/MapleMap/ItemDrop.cpp"
    "Gameplay/MapleMap/MapBackgrounds.cpp"
    "Gameplay/MapleMap/MapChars.cpp"
    "Gameplay/MapleMap/MapDrops.cpp"
    "Gameplay/MapleMap/MapEffect.cpp"
    "Gameplay/MapleMap/MapInfo.cpp"
    "Gameplay/MapleMap/MapMobs.cpp"
    "Gameplay/MapleMap/MapNpcs.cpp"
    "Gameplay/MapleMap/MapObject.cpp"
    "Gameplay/MapleMap/MapObjects.cpp"
    "Gameplay/MapleMap/MapPortals.cpp"
    "Gameplay/MapleMap/MapReactors.cpp"
    "Gameplay/MapleMap/MapTilesObjs.cpp"
    "Gameplay/MapleMap/MesoDrop.cpp"
    "Gameplay/MapleMap/Mob.cpp"
    "Gameplay/MapleMap/Npc.cpp"
    "Gameplay/MapleMap/Obj.cpp"
    "Gameplay/MapleMap/Portal.cpp"
    "Gameplay/MapleMap/Reactor.cpp"
    "Gameplay/MapleMap/Tile.cpp"
    "Gameplay/Physics/Foothold.cpp"
    "Gameplay/Physics/FootholdTree.cpp"
    "Gameplay/Physics/Physics.cpp"
    "Gameplay/Spawn.cpp"
    "Gameplay/Stage.cpp"
    "Graphics/Animation.cpp"
    "Graphics/Color.cpp"
    "Graphics/EffectLayer.cpp"
    "Graphics/Geometry.cpp"
    "Graphics/GraphicsGL.cpp"
    "Graphics/Sprite.cpp"
    "Graphics/Text.cpp"
    "Graphics/Texture.cpp"
    "IO/Components/AreaButton.cpp"
    "IO/Components/Button.cpp"
    "IO/Components/Charset.cpp"
    "IO/Components/ChatBalloon.cpp"
    "IO/Components/EquipTooltip.cpp"
    "IO/Components/Gauge.cpp"
    "IO/Components/Icon.cpp"
    "IO/Components/IconCover.cpp"
    "IO/Components/ItemTooltip.cpp"
    "IO/Components/MapleButton.cpp"
    "IO/Components/MapleComboBox.cpp"
    "IO/Components/MapleFrame.cpp"
    "IO/Components/MapTooltip.cpp"
    "IO/Components/NameTag.cpp"
    "IO/Components/NpcText.cpp"
    "IO/Components/ScrollingNotice.cpp"
    "IO/Components/SkillTooltip.cpp"
    "IO/Components/Slider.cpp"
    "IO/Components/StatefulIcon.cpp"
    "IO/Components/Textfield.cpp"
    "IO/Components/TextTooltip.cpp"
    "IO/Components/TwoSpriteButton.cpp"
    "IO/Cursor.cpp"
    "IO/Keyboard.cpp"
    "IO/Messages.cpp"
    "IO/UI.cpp"
    "IO/UIElement.cpp"
    "IO/UIStateCashShop.cpp"
    "IO/UIStateGame.cpp"
    "IO/UIStateLogin.cpp"
    "IO/UITypes/UIBuffList.cpp"
    "IO/UITypes/UICashShop.cpp"
    "IO/UITypes/UIChannel.cpp"
    "IO/UITypes/UICharInfo.cpp"
    "IO/UITypes/UICharSelect.cpp"
    "IO/UITypes/UIChat.cpp"
    "IO/UITypes/UIChatBar.cpp"
    "IO/UITypes/UICommonCreation.cpp"
    "IO/UITypes/UIEquipInventory.cpp"
    "IO/UITypes/UIEvent.cpp"
    "IO/UITypes/UIGender.cpp"
    "IO/UITypes/UIItemInventory.cpp"
    "IO/UITypes/UIJoypad.cpp"
    "IO/UITypes/UIKeyConfig.cpp"
    "IO/UITypes/UILogin.cpp"
    "IO/UITypes/UILoginNotice.cpp"
    "IO/UITypes/UILoginWait.cpp"
    "IO/UITypes/UILogo.cpp"
    "IO/UITypes/UIMiniMap.cpp"
    "IO/UITypes/UINotice.cpp"
    "IO/UITypes/UINpcTalk.cpp"
    "IO/UITypes/UIOptionMenu.cpp"
    "IO/UITypes/UIQuestLog.cpp"
    "IO/UITypes/UIQuit.cpp"
    "IO/UITypes/UIRaceSelect.cpp"
    "IO/UITypes/UIRegion.cpp"
    "IO/UITypes/UIShop.cpp"
    "IO/UITypes/UISkillBook.cpp"
    "IO/UITypes/UISoftKey.cpp"
    "IO/UITypes/UIStatsInfo.cpp"
    "IO/UITypes/UIStatusBar.cpp"
    "IO/UITypes/UIStatusMessenger.cpp"
    "IO/UITypes/UITermsOfService.cpp"
    "IO/UITypes/UIUserList.cpp"
    "IO/UITypes/UIWorldMap.cpp"
    "IO/UITypes/UIWorldSelect.cpp"
    "IO/UITypes/UIWorldSelectBackground.cpp"
    "IO/Window.cpp"
    "MapleStory.cpp"
    "Net/Cryptography.cpp"
    "Net/Handlers/AttackHandlers.cpp"
    "Net/Handlers/CashShopHandlers.cpp"
    "Net/Handlers/CommonHandlers.cpp"
    "Net/Handlers/Helpers/CashShopParser.cpp"
    "Net/Handlers/Helpers/CharacterParser.cpp"
    "Net/Handlers/Helpers/ItemParser.cpp"
    "Net/Handlers/Helpers/LoginParser.cpp"
    "Net/Handlers/Helpers/MovementParser.cpp"
    "Net/Handlers/InventoryHandlers.cpp"
    "Net/Handlers/LoginHandlers.cpp"
    "Net/Handlers/MapObjectHandlers.cpp"
    "Net/Handlers/MessagingHandlers.cpp"
    "Net/Handlers/NpcInteractionHandlers.cpp"
    "Net/Handlers/PlayerHandlers.cpp"
    "Net/Handlers/PlayerInteractionHandlers.cpp"
    "Net/Handlers/SetFieldHandlers.cpp"
    "Net/Handlers/TestingHandlers.cpp"
    "Net/InPacket.cpp"
    "Net/OutPacket.cpp"
    "Net/PacketSwitch.cpp"
    "Net/Session.cpp"
    "Net/SocketAsio.cpp"
    "Net/SocketWinsock.cpp"
    "Util/Misc.cpp"
    "Util/NxFiles.cpp"
    "Util/WzFiles.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE MapleStory)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/$ENV{PlatformTarget}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/$ENV{PlatformTarget}/$<CONFIG>/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/$ENV{PlatformTarget}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/$ENV{PlatformTarget}/$<CONFIG>/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.1.0/include/GL;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/freetype/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glfw-3.3.2.bin.WIN$ENV{PlatformArchitecture}/include/GLFW;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/stb;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/bass24/c;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/NoLifeNx"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.1.0/include/GL;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/freetype/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glfw-3.3.2.bin.WIN$ENV{PlatformArchitecture}/include/GLFW;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/stb;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/bass24/c;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/NoLifeNx"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
#            /permissive-;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
#            /permissive-;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /SUBSYSTEM:CONSOLE;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
#    add_custom_command_if(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMANDS
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\freetype\\win$ENV{PlatformArchitecture}\\freetype.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\bass24\\$ENV{PlatformTarget}\\bass.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\NoLifeNx\\nlnx\\includes\\lz4_v1_8_2_win$ENV{PlatformArchitecture}\\dll\\liblz4.so.1.8.2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>liblz4.dll"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\freetype\\win$ENV{PlatformArchitecture}\\freetype.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\bass24\\$ENV{PlatformTarget}\\bass.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\NoLifeNx\\nlnx\\includes\\lz4_v1_8_2_win$ENV{PlatformArchitecture}\\dll\\liblz4.so.1.8.2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>liblz4.dll"
#    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
#    add_custom_command_if(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMANDS
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\freetype\\win$ENV{PlatformArchitecture}\\freetype.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\bass24\\$ENV{PlatformTarget}\\bass.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND   $<CONFIG:Debug> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\NoLifeNx\\nlnx\\includes\\lz4_v1_8_2_win$ENV{PlatformArchitecture}\\dll\\liblz4.so.1.8.2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>liblz4.dll"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\freetype\\win$ENV{PlatformArchitecture}\\freetype.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\bass24\\$ENV{PlatformTarget}\\bass.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>"
#        COMMAND $<CONFIG:Release> copy /Y /D "$<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>includes\\NoLifeNx\\nlnx\\includes\\lz4_v1_8_2_win$ENV{PlatformArchitecture}\\dll\\liblz4.so.1.8.2.dll" "$<SHELL_PATH:${OUTPUT_DIRECTORY}>liblz4.dll"
#    )
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "glew32s;"
        "freetype;"
        "glfw3;"
        "bass;"
        "OpenGL32;"
        "Iphlpapi"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "glew32s;"
        "freetype;"
        "glfw3;"
        "bass;"
        "OpenGL32;"
        "Iphlpapi;"
        "NoLifeNx;"
        "WzLib"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.1.0/lib/Release/${CMAKE_VS_PLATFORM_NAME};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/freetype/win$ENV{PlatformArchitecture};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glfw-3.3.2.bin.WIN$ENV{PlatformArchitecture}/lib-vc2015;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/bass24/c/$ENV{PlatformTarget};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/NoLifeNx/nlnx/$ENV{PlatformTarget}/$<CONFIG>;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/WzLib/bin/Win32Release"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glew-2.1.0/lib/Release/${CMAKE_VS_PLATFORM_NAME};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/freetype/win$ENV{PlatformArchitecture};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/glfw-3.3.2.bin.WIN$ENV{PlatformArchitecture}/lib-vc2015;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/bass24/c/$ENV{PlatformTarget};"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/NoLifeNx/nlnx/$ENV{PlatformTarget}/$<CONFIG>;"
        "${CMAKE_CURRENT_SOURCE_DIR}/includes/WzLib/bin/Win32Release"
    )
endif()

