////////////////////////////////////////////////////////////////////
//
// portal2.spa.h
//
// Auto-generated on Monday, 10 March 2025 at 17:25:19
// XLAST project version 1.0.57.0
// SPA Compiler version 2.0.20500.0
//
// This file has been edited manually because I couldn't get avatar awards working in the actual xlast tool.
// Some IDs may be wrong, in cases where it wasn't possible to deduce them from the PC binaries.
// Ask Kelsey if you need the actual .xlast file, I didn't want to put it on a public repository.
//
////////////////////////////////////////////////////////////////////

#ifndef __PORTAL_2_SPA_H__
#define __PORTAL_2_SPA_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Title info
//

#define TITLEID_PORTAL_2                            0x45410912

//
// Context ids
//
// These values are passed as the dwContextId to XUserSetContext.
//

#define CONTEXT_COOP_PRESENCE_TRACK                 0
#define CONTEXT_SP_PRESENCE_TEXT                    4
#define CONTEXT_COOP_PRESENCE_TAGLINE               5
#define CONTEXT_COOP_PRESENCE_WAITING               6

//
// Context values
//
// These values are passed as the dwContextValue to XUserSetContext.
//

// Values for CONTEXT_COOP_PRESENCE_TRACK

#define CONTEXT_COOP_PRESENCE_TRACK_DEFAULT         0

// Values for CONTEXT_SP_PRESENCE_TEXT

#define CONTEXT_SP_PRESENCE_TEXT_DEFAULT            0
#define CONTEXT_SP_PRESENCE_TEXT_CH1                1
#define CONTEXT_SP_PRESENCE_TEXT_CH2                2
#define CONTEXT_SP_PRESENCE_TEXT_CH3                3
#define CONTEXT_SP_PRESENCE_TEXT_CH4                4
#define CONTEXT_SP_PRESENCE_TEXT_CH5                5
#define CONTEXT_SP_PRESENCE_TEXT_CH6                6
#define CONTEXT_SP_PRESENCE_TEXT_CH7                7
#define CONTEXT_SP_PRESENCE_TEXT_CH8                8
#define CONTEXT_SP_PRESENCE_TEXT_CH9                9
#define CONTEXT_SP_PRESENCE_TEXT_CH10               10

// Values for CONTEXT_COOP_PRESENCE_TAGLINE

#define CONTEXT_COOP_PRESENCE_TAGLINE_DEFAULT       0
#define CONTEXT_COOP_PRESENCE_TAGLINE_SPLITSCREEN   1
#define CONTEXT_COOP_PRESENCE_TAGLINE_SYSTEMLINK    2
#define CONTEXT_COOP_PRESENCE_TAGLINE_FRIEND        3
#define CONTEXT_COOP_PRESENCE_TAGLINE_QUICKMATCH    4

// Values for CONTEXT_COOP_PRESENCE_WAITING

#define CONTEXT_COOP_PRESENCE_WAITING_DEFAULT       0
#define CONTEXT_COOP_PRESENCE_WAITING_SPLITSCREEN   1
#define CONTEXT_COOP_PRESENCE_WAITING_SYSTEMLINK    2
#define CONTEXT_COOP_PRESENCE_WAITING_FRIEND        3
#define CONTEXT_COOP_PRESENCE_WAITING_QUICKMATCH    4

// Values for X_CONTEXT_PRESENCE

#define CONTEXT_PRESENCE_MAINMENU                   0
#define CONTEXT_PRESENCE_SP                         1
#define CONTEXT_PRESENCE_COOPGAME_TRACK             2
#define CONTEXT_PRESENCE_COOPGAME                   3
#define CONTEXT_PRESENCE_COOPMENU                   4

// Values for X_CONTEXT_GAME_MODE

#define CONTEXT_GAME_MODE_SP                        0
#define CONTEXT_GAME_MODE_COOP                      1
#define CONTEXT_GAME_MODE_COOP_CHALLENGE            2
#define CONTEXT_GAME_MODE_COOP_COMMUNITY            3

//
// Property ids
//
// These values are passed as the dwPropertyId value to XUserSetProperty
// and as the dwPropertyId value in the XUSER_PROPERTY structure.
//

#define PROPERTY_REQUIRED_DLC1                      0x10000001
#define PROPERTY_MMVERSION                          0x10000002
#define PROPERTY_COOP_TRACK_CHAPTER                 0x10000003
#define PROPERTY_COOP_TRACK_NUMCHAPTERS             0x10000004

//
// Achievement ids
//
// These values are used in the dwAchievementId member of the
// XUSER_ACHIEVEMENT structure that is used with
// XUserWriteAchievements and XUserCreateAchievementEnumerator.
//

#define ACHIEVEMENT_SURVIVE_CONTAINER_RIDE          1
#define ACHIEVEMENT_WAKE_UP                         2
#define ACHIEVEMENT_LASER                           3
#define ACHIEVEMENT_BRIDGE                          4
#define ACHIEVEMENT_BREAK_OUT                       5
#define ACHIEVEMENT_STALEMATE_ASSOCIATE             6
#define ACHIEVEMENT_ADDICTED_TO_SPUDS               7
#define ACHIEVEMENT_BLUE_GEL                        8
#define ACHIEVEMENT_ORANGE_GEL                      9
#define ACHIEVEMENT_WHITE_GEL                       10
#define ACHIEVEMENT_TRACTOR_BEAM                    11
#define ACHIEVEMENT_TRIVIAL_TEST                    12
#define ACHIEVEMENT_WHEATLEY_TRIES_TO               13
#define ACHIEVEMENT_SHOOT_THE_MOON                  14
#define ACHIEVEMENT_BOX_HOLE_IN_ONE                 15
#define ACHIEVEMENT_SPEED_RUN_LEVEL                 16
#define ACHIEVEMENT_COMPLIANT                       17
#define ACHIEVEMENT_SAVE_CUBE                       18
#define ACHIEVEMENT_LAUNCH_TURRET                   19
#define ACHIEVEMENT_CLEAN_UP                        20
#define ACHIEVEMENT_REENTER_TEST_CHAMBERS           21
#define ACHIEVEMENT_NOT_THE_DROID                   22
#define ACHIEVEMENT_SAVE_REDEMPTION_TURRET          23
#define ACHIEVEMENT_CATCH_CRAZY_BOX                 24
#define ACHIEVEMENT_NO_BOAT                         25
#define ACHIEVEMENT_A3_DOORS                        26
#define ACHIEVEMENT_PORTRAIT                        27
#define ACHIEVEMENT_DEFIANT                         28
#define ACHIEVEMENT_BREAK_MONITORS                  29
#define ACHIEVEMENT_HI_FIVE_YOUR_PARTNER            30
#define ACHIEVEMENT_TEAM_BUILDING                   31
#define ACHIEVEMENT_MASS_AND_VELOCITY               32
#define ACHIEVEMENT_HUG_NAME                        33
#define ACHIEVEMENT_EXCURSION_FUNNELS               34
#define ACHIEVEMENT_NEW_BLOOD                       35
#define ACHIEVEMENT_NICE_CATCH                      36
#define ACHIEVEMENT_TAUNTS                          37
#define ACHIEVEMENT_YOU_MONSTER                     38
#define ACHIEVEMENT_PARTNER_DROP                    39
#define ACHIEVEMENT_PARTY_OF_THREE                  40
#define ACHIEVEMENT_PORTAL_TAUNT                    41
#define ACHIEVEMENT_TEACHER                         42
#define ACHIEVEMENT_WITH_STYLE                      43
#define ACHIEVEMENT_LIMITED_PORTALS                 44
#define ACHIEVEMENT_FOUR_PORTALS                    45
#define ACHIEVEMENT_SPEED_RUN_COOP                  46
#define ACHIEVEMENT_STAYING_ALIVE                   47
#define ACHIEVEMENT_TAUNT_CAMERA                    48
#define ACHIEVEMENT_ROCK_CRUSHES_ROBOT              49
#define ACHIEVEMENT_SPREAD_THE_LOVE                 50
#define ACHIEVEMENT_SUMMER_SALE                     51

//
// AvatarAssetAward ids
//

#define AVATARASSETAWARD_SHIRT1                     1
#define AVATARASSETAWARD_SHIRT2                     2
#define AVATARASSETAWARD_SHIRT3                     3
#define AVATARASSETAWARD_HAT1                       4
#define AVATARASSETAWARD_PROP1                      5

//
// Stats view ids
//
// These are used in the dwViewId member of the XUSER_STATS_SPEC structure
// passed to the XUserReadStats* and XUserCreateStatsEnumerator* functions.
//

// Skill leaderboards for ranked game modes

#define STATS_VIEW_SKILL_RANKED_SP                  0xFFFF0000
#define STATS_VIEW_SKILL_RANKED_COOP                0xFFFF0001
#define STATS_VIEW_SKILL_RANKED_COOP_CHALLENGE      0xFFFF0002
#define STATS_VIEW_SKILL_RANKED_COOP_COMMUNITY      0xFFFF0003

// Skill leaderboards for unranked (standard) game modes

#define STATS_VIEW_SKILL_STANDARD_SP                0xFFFE0000
#define STATS_VIEW_SKILL_STANDARD_COOP              0xFFFE0001
#define STATS_VIEW_SKILL_STANDARD_COOP_CHALLENGE    0xFFFE0002
#define STATS_VIEW_SKILL_STANDARD_COOP_COMMUNITY    0xFFFE0003

// Title defined leaderboards


//
// Stats view column ids
//
// These ids are used to read columns of stats views.  They are specified in
// the rgwColumnIds array of the XUSER_STATS_SPEC structure.  Rank, rating
// and gamertag are not retrieved as custom columns and so are not included
// in the following definitions.  They can be retrieved from each row's
// header (e.g., pStatsResults->pViews[x].pRows[y].dwRank, etc.).
//

//
// Matchmaking queries
//
// These values are passed as the dwProcedureIndex parameter to
// XSessionSearch to indicate which matchmaking query to run.
//

#define SESSION_MATCH_QUERY_COOP                    0

//
// Gamer pictures
//
// These ids are passed as the dwPictureId parameter to XUserAwardGamerTile.
//



#ifdef __cplusplus
}
#endif

#endif // __PORTAL_2_SPA_H__


