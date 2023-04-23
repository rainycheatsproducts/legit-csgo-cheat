#include "imports.hpp"
mem driver ( utils::globals::process_id );
bool showing = true;
using namespace offsets;

namespace process
{
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	int WindowLeft;
	int WindowRight;
	int WindowTop;
	int WindowBottom;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
}

namespace overlay_window
{
	WNDCLASSEX WindowClass;
	HWND Hwnd;
	LPCSTR Name;
}

namespace directx
{
	IDirect3D9Ex* Direct3D9 = NULL;
	IDirect3DDevice9Ex* pDevice = NULL;
	D3DPRESENT_PARAMETERS pParams = { NULL };
	MARGINS Margin = { -1 };
	MSG Message = { NULL };
}

namespace imgui
{
	std::array< std::string, 256 > keys =
	{
		"bind",
		"LMB",
		"RMB",
		"Control B",
		"MMB",
		"X1 Mouse",
		"X2 Mouse",
		"Undefined",
		"Backspace",
		"TAB",
		"Reserved",
		"Reserved",
		"Clear",
		"Enter",
		"Reserved",
		"Reserved",
		"Shift",
		"Control",
		"Alt",
		"Pause",
		"CAPS Lock",
		"IME",
		"IME",
		"IME",
		"IME",
		"IME",
		"IME",
		"ESC",
		"IME",
		"IME",
		"IME",
		"IME",
		"Spacebar",
		"Page Up",
		"Page Down",
		"End",
		"Home",
		"Left Arrow",
		"Up Arrow",
		"Right Arrow",
		"Down Arrow",
		"Select",
		"Print",
		"Execute",
		"Print Screen",
		"Insert",
		"Delete",
		"Help",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"Undefined",
		"Undefined",
		"Undefined",
		"Undefined",
		"Undefined",
		"Undefined",
		"Undefined",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"Left Window",
		"Right Window",
		"Applications",
		"Reserved",
		"Sleep",
		"Num 0",
		"Num 1",
		"Num 2",
		"Num 3",
		"Num 4",
		"Num 5",
		"Num 6",
		"Num 7",
		"Num 8",
		"Num 9",
		"Num *",
		"Num +",
		"Num Sep",
		"Num -",
		"Num .",
		"Num /",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F13",
		"F14",
		"F15",
		"F16",
		"F17",
		"F18",
		"F19",
		"F20",
		"F21",
		"F22",
		"F23",
		"F24",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Numlock",
		"Scroll",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"Left Shift",
		"Right Shift",
		"Left Control",
		"Right Control",
		"Left Menu",
		"Right Menu",
		"Browser back",
		"Browser forward",
		"Browser refresh",
		"Browser stop",
		"Browser search",
		"Browser favorites",
		"Browser home",
		"Volume mute",
		"Volume down",
		"Volume up",
		"Next track",
		"Previous track",
		"Stop track",
		"Play/Pause track",
		"Start mail",
		"Select media",
		"Start app1",
		"Start app2",
		"Reserved",
		"Reserved",
		";:",
		"+",
		",",
		"-",
		".",
		"/?",
		"`~",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Unassigned",
		"Unassigned",
		"Unassigned",
		"[{",
		"\\|",
		"]}",
		"\'\"",
		"OEM",
		"Reserved",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"IME",
		"OEM",
		"VK_PACKET",
		"Unassigned",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"OEM",
		"Attn",
		"CrSel",
		"ExSel",
		"EOF",
		"Play",
		"Zoom",
		"Noname",
		"PA1",
		"Clear",
		"None"
	};

	void hot_key ( const char* str, int* k )
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow ( );
		if ( window->SkipItems )
			return;

		const ImGuiID id = window->GetID ( str );
		static std::map<ImGuiID, bool> wait_table;
		if ( wait_table.find ( id ) == wait_table.end ( ) )
			wait_table.emplace ( id, false );

		auto& waiting = wait_table[ id ];

		ImVec2 size = { 6, 4 };

		if ( !waiting && ImGui::Button ( keys[ *k ].c_str ( ), ImGui::CalcTextSize ( keys[ *k ].c_str ( ) ) + size ) )
			waiting = true;
		else if ( waiting )
		{
			ImGui::Button ( "...", { 62, 18 } );
			std::this_thread::sleep_for ( std::chrono::milliseconds ( 75 ) );

			for ( auto i = 0; i < keys.size ( ); ++i )
			{
				if ( !GetAsyncKeyState ( i ) )
					continue;

				if ( GetAsyncKeyState ( VK_ESCAPE ) )
				{
					*k = 0;
					waiting = false;
				}
				else
				{
					*k = i;
					waiting = false;
				}
			}
		}
	}

	struct tab_anim
	{
		float active_text_anim;
		float active_rect_alpha;
	};

	auto tab ( const char* label, bool selected ) -> bool
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow ( );
		if ( window->SkipItems )
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID ( label );
		const ImVec2 label_size = ImGui::CalcTextSize ( label );
		ImVec2 pos = window->DC.CursorPos;

		const ImRect rect ( pos, ImVec2 ( pos.x + label_size.x + 10, pos.y + 23 ) );
		ImGui::ItemSize ( ImVec4 ( rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y ), style.FramePadding.y );
		if ( !ImGui::ItemAdd ( rect, id ) )
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior ( rect, id, &hovered, &held, NULL );

		static std::map <ImGuiID, tab_anim> anim;
		auto it_anim = anim.find ( id );
		if ( it_anim == anim.end ( ) )
		{
			anim.insert ( { id, {0.5f, 0.0f} } );
			it_anim = anim.find ( id );
		}

		it_anim->second.active_text_anim = ImLerp ( it_anim->second.active_text_anim, selected ? 1.0f : hovered ? 0.8f : 0.5f, 0.06f * ( 1.0f - ImGui::GetIO ( ).DeltaTime ) );
		it_anim->second.active_rect_alpha = ImLerp ( it_anim->second.active_rect_alpha, selected ? 1.0f : 0.0f, 0.08f * ( 1.0f - ImGui::GetIO ( ).DeltaTime ) );

		window->DrawList->AddRectFilled ( ImVec2 ( rect.Min.x, rect.Max.y + 10 ), ImVec2 ( rect.Max.x, rect.Max.y + 12 ), ImColor ( 103 / 255.0f, 104 / 255.0f, 255 / 255.0f, it_anim->second.active_rect_alpha ), 2.0f, ImDrawFlags_RoundCornersTop );
		window->DrawList->AddText ( ImVec2 ( ( rect.Min.x + rect.Max.x ) / 2 - ( label_size.x ) / 2 - 1, ( rect.Min.y + rect.Max.y ) / 2.f - ( label_size.y / 2.f ) ), ImColor ( 1.0f, 1.0f, 1.0f, it_anim->second.active_text_anim ), label );

		return pressed;
	}

}

namespace sdk
{
	enum player_bones
	{
		head = 8,
		neck = 7,
		l_shoulder = 41,
		r_shoulder = 11,
		u_spine = 6,
		m_spine = 5,
		l_spine = 4,
		pelvis = 0,
		l_knee = 78,
		r_knee = 71,
		l_foot = 72,
		r_foot = 79
	};

	enum player_flags : int
	{
		on_ground = ( 1 << 0 ),
		ducking = ( 1 << 1 ),
		water_jump = ( 1 << 3 ),
		on_train = ( 1 << 4 ),
		in_train = ( 1 << 5 ),
		frozen = ( 1 << 6 ),
		at_controls = ( 1 << 7 ),
		client = ( 1 << 8 ),
		fake_client = ( 1 << 9 ),
		in_wtaer = ( 1 << 10 ),
	};

	enum weapon_id
	{
		weapon_none,
		weapon_deagle,
		weapon_elite,
		weapon_fiveseven,
		weapon_glock,
		weapon_p228,
		weapon_usp,
		weapon_ak47,
		weapon_aug,
		weapon_awp,
		weapon_famas,
		weapon_g3sg1,
		weapon_galil,
		weapon_galilar,
		weapon_m249,
		weapon_m3,
		weapon_m4a1,
		weapon_mac10,
		weapon_mp5navy,
		weapon_p90,
		weapon_scout,
		weapon_sg550,
		weapon_sg552,
		weapon_tmp,
		weapon_ump45,
		weapon_xm1014,
		weapon_bizon,
		weapon_mag7,
		weapon_negev,
		weapon_sawedoff,
		weapon_tec9,
		weapon_taser,
		weapon_hkp2000,
		weapon_mp7,
		weapon_mp9,
		weapon_nova,
		weapon_p250,
		weapon_scar17,
		weapon_scar20,
		weapon_sg556,
		weapon_ssg08,
		weapon_knifegg,
		weapon_knife_dupe,
		weapon_flashbang,
		weapon_hegrenade,
		weapon_smokegrenade,
		weapon_molotov,
		weapon_decoy,
		weapon_incgrenade,
		weapon_c4
	};

	enum knife_id
	{
		weapon_knife = 42,
		weapon_knife_t = 59,
		weapon_knife_bayonet = 500,
		weapon_knife_flip = 505,
		weapon_knife_gut = 506,
		weapon_knife_karambit = 507,
		weapon_knife_m9_bayonet = 508,
		weapon_knife_tactical = 509,
		weapon_knife_falchion = 512,
		weapon_knife_survival_bowie = 514,
		weapon_knife_butterfly = 515,
		weapon_knife_push = 516,
		weapon_knife_ursus = 519,
		weapon_knife_gypsy_jackknife = 520,
		weapon_knife_stiletto = 522,
		weapon_knife_widomaker = 523
	};

	enum ak47_skins
	{
		red_laminate = 14,
		case_hardened = 44,
		safari_mesh = 72,
		jungle_spray = 122,
		predator = 170,
		black_laminate = 172,
		fire_serpent = 180,
		blue_laminate = 226,
		redline = 282,
		emerald_pinstripe = 300,
		vulcan = 302,
		jaguar = 316,
		jet_set = 340,
		first_class = 341,
		wasteland_rebel = 380,
		cartel = 394,
		elite_build = 422,
		hydroponic = 456,
		aquamarine_revenge = 474,
		frontside_misty = 490,
		point_disarray = 506,
		fuel_injector = 524,
		neon_revolution = 600,
		bloodsport = 639,
		orbit_mk01 = 656,
		the_empress = 675,
		neon_rider = 707,
		wild_lotus = 724,
		baroque_purple = 745,
		safety_net = 795,
		asiimov_ak = 801,
		uncharted = 836,
		rat_rod = 885,
		gold_arabesque = 921,
		phantom_disruptor = 941,
		legion_of_anubis = 959,
		xray_ak = 1004,
		panthera_onca = 1018,
		slate = 1035,
		green_laminate = 1070,
		leet_museo = 1087,
		nightwish = 1141,
		ice_coaled = 1143,
		head_shot = 1221
	};

	enum m4a4_skins
	{
		desert_storm = 8,
		jungle_tiger = 16,
		urban_ddpat = 17,
		tornado = 101,
		bullet_rain = 155,
		modern_hunter = 164,
		radiation_hazard = 167,
		faded_zebra = 176,
		zirka = 187,
		xray_m4 = 215,
		asiimov_m4 = 255,
		howl = 309,
		desertstrike = 336,
		griffin = 384,
		_dragon_king = 400,
		poseidon = 449,
		daybreak = 471,
		evil_daimyo = 480,
		royal_paladin = 512,
		the_battlestar = 533,
		desolate_space = 588,
		buzz_kill = 632,
		hellfire = 664,
		neonoir = 695,
		dark_blossom = 730,
		mainframe = 780,
		converter = 793,
		magnesium = 811,
		the_emperor = 844,
		red_ddpat = 926,
		tooth_fairy = 971,
		cyber_security = 985,
		global_offensive = 993,
		in_living_color = 1041,
		the_coalition = 1063,
		spider_lily = 1097,
		poly_mag = 1149,
		temukau = 1228
	};

	enum m4a1s_skins
	{
		dark_water = 60,
		boreal_forest = 77,
		bright_water = 189,
		blood_tiger = 217,
		varicamo = 235,
		nitro = 254,
		guardian = 257,
		atomic_alloy = 301,
		master_piece = 321,
		knight = 326,
		cyrex = 360,
		basilisk = 383,
		hyper_beast = 430,
		icarus_fell = 440,
		hot_rod = 445,
		golden_coil = 497,
		chanticos_fire = 548,
		mecha_industries = 587,
		flashback = 631,
		decimator = 644,
		briefing = 663,
		leaded_glass = 681,
		nightmare = 714,
		control_panel = 792,
		moss_quartz = 862,
		player_two = 946,
		printstream = 984,
		welcome_to_the_jungle = 1001,
		blue_phosphor = 1017,
		fizzy_pop = 1059,
		imminent_danger = 1073,
		night_terror = 1130,
		emphorosaurs = 1223
	};

	enum usps_skins
	{
		forest_leaves_usp = 25,
		dark_water_usp = 60,
		overgrowth_usp = 183,
		blood_tiger_usp = 217,
		serum_usp = 221,
		night_ops_usp = 236,
		stainless_usp = 277,
		guardian_usp = 290,
		orion_usp = 313,
		road_rash_usp = 318,
		royal_blue_usp = 332,
		caiman_usp = 339,
		business_class_usp = 364,
		pathfinder_usp = 443,
		para_green_usp = 454,
		torque_usp = 489,
		kill_confirmed_usp = 504,
		lead_conduit_usp = 540,
		cyrex_usp = 637,
		neonoir_usp = 653,
		blueprint_usp = 657,
		cortex_usp = 705,
		check_engine_usp = 796,
		flashback_usp = 817,
		purple_ddpat_usp,
		orange_anolis_usp = 922,
		monster_mashup_usp = 991,
		target_acquired_usp = 1027,
		ancient_visions_usp = 1031,
		the_traitor_usp = 1040,
		whiteout_usp = 1065,
		black_lotus_usp = 1102,
		ticket_to_hell_usp = 1136,
		printstream_usp = 1142
	};

	enum awupa_skins
	{
		snake_camo_awp = 30,
		lightning_strike_awp = 51,
		safari_mesh_awp = 72,
		pink_ddpat_awp = 84,
		boom_awp = 174,
		corticera_awp = 181,
		graphite_awp = 212,
		electric_hive_awp = 227,
		pit_viper_awp = 251,
		redline_awp = 259,
		asiimov_awp = 279,
		dragon_lore_awp = 344,
		manowar_awp = 395,
		worm_god_awp = 424,
		medusa_awp = 446,
		sun_in_leo_awp = 451,
		hyper_beast_awp = 475,
		elite_build_awp = 525,
		phobos_awp = 584,
		fever_dream_awp = 640,
		oni_taiji_awp = 662,
		mortis_awp = 691,
		paw_awp = 718,
		the_prince_awp = 736,
		gungnir_awp = 756,
		acheron_awp = 788,
		neonoir_awp = 803,
		desert_hydra_awp = 819,
		atheris_awp = 838,
		containment_breach_awp = 887,
		wildfire_awp = 917,
		capillary_awp = 943,
		exoskeleton_awp = 975,
		fade_awp = 1026,
		silk_tiger_awp = 1029,
		pop_awp = 1058,
		chromatic_aberration_awp = 1144,
		duality_awp = 1222
	};

	enum glock_skins
	{
		groundwater = 2,
		candy_apple = 3,
		fade = 38,
		night = 40,
		dragon_tattoo = 48,
		pink_ddpat = 84,
		brass = 159,
		sand_dune = 208,
		steel_disruption = 230,
		blue_fissure = 278,
		death_rattle = 293,
		water_elemental = 353,
		reactor = 367,
		grinder = 381,
		catacombs = 399,
		twilight_galaxy = 437,
		bunsen_burner = 479,
		wraiths = 495,
		royal_legion = 532,
		wasteland_rebel_glock = 586,
		weasel = 607,
		ironwork = 623,
		off_world = 680,
		moonrise = 694,
		warhawk = 713,
		synth_leaf = 732,
		nuclear_garden = 789,
		high_beam = 799,
		oxide_blaze = 808,
		sacrifice = 918,
		bullet_queen = 957,
		vogue = 963,
		neonoir_glock	 = 988,
		franklin = 1016,
		clear_polymer = 1039,
		red_tire = 1079,
		snack_attack = 1100,
		gamma_doppler_emerald = 1119,
		gamma_doppler_phase_1 = 1120,
		gamma_doppler_phase_2 = 1121,
		gamma_doppler_phase_3 = 1122,
		gamma_doppler_phase_4 = 1123,
		winterized = 1158,
		umbral_rabbit = 1227
	};

	const char* ak47_names[ 44 ]
	{
		"red laminate",
		"case hardened",
		"safari mesh",
		"jungle spray",
		"predator",
		"black laminate",
		"fire serpent",
		"blue laminate",
		"redline",
		"emerald pinstripe",
		"vulcan",
		"jaguar",
		"jet set",
		"first class",
		"wasteland rebel",
		"cartel",
		"elite build",
		"hydroponic",
		"aquamarine revenge",
		"frontside misty",
		"point disarray",
		"fuel injecto",
		"neon revolution",
		"bloodsport",
		"orbit mk01",
		"the empress",
		"neon rider",
		"wild lotus",
		"baroque purple",
		"safety net",
		"asiimov",
		"uncharted",
		"rat rod",
		"gold arabesque",
		"phantom disrupto",
		"legion of anubi",
		"xray",
		"panthera onca",
		"slate",
		"green laminate",
		"leet museo",
		"nightwish",
		"ice coaled",
		"head shot"
	};

	const char* m4a4_skins[ 38 ]
	{
		"desert storm",
		"jungle tiger",
		"urban ddpat",
		"tornado",
		"bullet rain",
		"modern hunter",
		"radiation hazard",
		"faded zebra",
		"zirka",
		"xray",
		"asiimov",
		"howl",
		"desertstrike",
		"griffin",
		"dragon king",
		"poseidon",
		"daybreak",
		"evil daimyo",
		"royal paladin",
		"the battlestar",
		"desolate space",
		"buzz kill",
		"hellfire",
		"neonoir",
		"dark blossom",
		"mainframe",
		"converter",
		"magnesium",
		"the emperor",
		"red ddpat",
		"tooth fairy",
		"cyber security",
		"global offensive",
		"in living color",
		"the coalition",
		"spider lily",
		"poly mag",
		"temukau"
	};

	const char* m4a1s_skins[ 33 ]
	{
		"dark water",
		"boreal forest",
		"bright water",
		"blood tiger",
		"varicamo",
		"nitro",
		"guardian",
		"atomic alloy",
		"master piece",
		"knight",
		"cyrex",
		"basilisk",
		"hyper beast",
		"icarus fell",
		"hot rod",
		"golden coil",
		"chanticos fire",
		"mecha industries",
		"flashback",
		"decimator",
		"briefing",
		"leaded glass",
		"nightmare",
		"control panel",
		"moss quartz",
		"player two",
		"printstream",
		"welcome to the jungle",
		"blue phosphor",
		"fizzy pop",
		"imminent danger",
		"night terror",
		"emphorosaurs"
	};

	const char* usps_skins[ 34 ]
	{
		"forest_leaves",
		"dark_water",
		"overgrowth",
		"blood_tiger",
		"serum",
		"night_ops",
		"stainless",
		"guardian",
		"orion",
		"road_rash",
		"royal_blue",
		"caiman",
		"business_class",
		"pathfinder",
		"para_green",
		"torque",
		"kill_confirmed",
		"lead_conduit",
		"cyrex",
		"neonoir",
		"blueprint",
		"cortex",
		"check_engine",
		"flashback",
		"purple_ddpat",
		"orange_anolis",
		"monster_mashup",
		"target_acquired",
		"ancient_visions",
		"the_traitor",
		"whiteout",
		"black_lotus",
		"ticket_to_hell",
		"printstream"
	};

	const char* awupa_names[ 38 ]
	{
		"snake camo",
		"lightning strike",
		"safari mesh",
		"pink ddpat",
		"boom",
		"corticera",
		"graphite",
		"electric hive",
		"pit viper",
		"redline",
		"asiimov",
		"dragon lore",
		"manowar",
		"worm god",
		"medusa",
		"sun in leo",
		"hyper beast",
		"elite build",
		"phobos",
		"fever dream",
		"oni taiji",
		"mortis",
		"paw",
		"the prince",
		"gungnir",
		"acheron",
		"neonoir",
		"desert hydra",
		"atheris",
		"containment breach",
		"wildfire",
		"capillary",
		"exoskeleton",
		"fade",
		"silk tiger",
		"pop awp",
		"chromatic aberration",
		"duality"
	};

	const char* glock_names[ 44 ]
	{
		"groundwater",
		"candy apple",
		"fade",
		"night",
		"dragon tattoo",
		"pink ddpat",
		"brass",
		"sand dune",
		"steel disruption",
		"blue fissure",
		"death rattle",
		"water elemental",
		"reactor",
		"grinder",
		"catacombs",
		"twilight galaxy",
		"bunsen burner",
		"wraiths",
		"royal legion",
		"wasteland rebel",
		"weasel",
		"ironwork",
		"off world",
		"moonrise",
		"warhawk",
		"synth leaf",
		"nuclear garden",
		"high beam",
		"oxide blaze",
		"sacrifice",
		"bullet queen",
		"vogue",
		"neonoir",
		"franklin",
		"clear polymer",
		"red tire",
		"snack attack",
		"gamma doppler emerald",
		"gamma doppler phase 1",
		"gamma doppler phase 2",
		"gamma doppler phase 3",
		"gamma doppler phase 4",
		"winterized",
		"umbral rabbit"
	};

	namespace globals
	{
		auto width = GetSystemMetrics ( SM_CXSCREEN );
		auto height = GetSystemMetrics ( SM_CYSCREEN );
		namespace aimbot
		{
			auto bind = 0;
			auto x360 = false;
			auto enabled = false;
			auto aim_fov = 10.00f;
			auto smoothing = 1.00f;
		}

		namespace localplayer
		{
			auto bhop = false;
			auto no_flash = false;
			auto no_recoil = false;
			auto skin_changer = false;
			auto local_fov = 68.00f;
		}

		namespace visuals
		{
			auto esp = false;
			auto box = false;
			auto glow = false;
			auto name = false;
			auto health = false;
			auto tracers = false;
			Color color_glow_allyOC = { 0, 0, 0 };
			Color color_glow_enemyOC = { 0, 0, 0 };
			Color teamColor = Color{ 0,0,255 };
			Color enemyColor = Color{ 255,0,0 };
		}
	}

	namespace rifles
	{
		auto ak47 = 0;
		auto m4a4 = 0;
		auto m4a1s = 0;
		auto famas = 0;
		auto aug = 0;
		auto galil = 0;
		auto sg553 = 0;
	}

	namespace pistols
	{
		auto usps = 0;
		auto revolver = 0;
		auto cz75_auto = 0;
		auto deagle = 0;
		auto dual_berettas = 0;
		auto five_seven = 0;
		auto glock = 0;
		auto p2000 = 0;
		auto p250 = 0;
		auto tec9 = 0;
	}

	namespace smgs
	{
		auto mac10 = 0;
		auto mp7 = 0;
		auto mp9 = 0;
		auto mp5 = 0;
	}

	namespace shotguns
	{
		auto mag7 = 0;
		auto nova = 0;
		auto sawed_off = 0;
		auto xm1014 = 0;
		auto m249 = 0;
		auto negev = 0;
	}

	namespace snupers
	{
		auto awp = 0;
		auto g32g1 = 0;
		auto scar20 = 0;
		auto ssg08 = 0;
	}

	UINT knife_index = 0;

	namespace functions
	{
		auto calculate_angle
		(
			geo::vec3_t localPosition,
			geo::vec3_t enemyPosition,
			geo::vec3_t viewAngles
		) -> geo::vec3_t
		{
			return ( ( enemyPosition - localPosition ).to_angle ( ) - viewAngles );
		}

		auto world_to_screen ( geo::vec3_t& world, geo::vec2_t& screen, view_matrix_t matrix ) -> bool
		{
			screen.x = matrix[ 0 ][ 0 ] * world.x + matrix[ 0 ][ 1 ] * world.y + matrix[ 0 ][ 2 ] * world.z + matrix[ 0 ][ 3 ];
			screen.y = matrix[ 1 ][ 0 ] * world.x + matrix[ 1 ][ 1 ] * world.y + matrix[ 1 ][ 2 ] * world.z + matrix[ 1 ][ 3 ];

			float w = matrix[ 3 ][ 0 ] * world.x + matrix[ 3 ][ 1 ] * world.y + matrix[ 3 ][ 2 ] * world.z + matrix[ 3 ][ 3 ];

			if ( w < 0.01f )
				return false;

			float inv_w = 1.f / w;
			screen.x *= inv_w;
			screen.y *= inv_w;

			float x = ( float )globals::width * .5f;
			float y = ( float )globals::height * .5f;

			x += 0.5f * screen.x * ( float )globals::width + 0.5f;
			y -= 0.5f * screen.y * ( float )globals::height + 0.5f;

			screen.x = x;
			screen.y = y;

			return true;
		}

		auto get_bone_pos ( int pTargetIn, int bone ) -> geo::vec3_t
		{
			const int matrix = driver.read<int> ( pTargetIn + netvars::m_dwBoneMatrix );
			return geo::vec3_t
			(
				driver.read<float> ( matrix + 0x30 * bone + 0xC ),
				driver.read<float> ( matrix + 0x30 * bone + 0x1C ),
				driver.read<float> ( matrix + 0x30 * bone + 0x2C )
			);
		}

		auto get_weapon_paint ( int item_definition ) -> int
		{
			switch ( item_definition )
			{
				case 7: return rifles::ak47;
				case 16: return rifles::m4a4;
				case 60: return rifles::m4a1s;

				case 9: return snupers::awp;

				case 4: return pistols::glock;
				case 61: return pistols::usps;
				default: return 0;
			}
		}

		auto display_name ( int Index ) -> std::string
		{
			struct PlayerInfo_t
			{
				char __pad[ 0x10 ];
				char name[ 32 ];
			};

			std::uintptr_t ClientState = driver.read< std::uintptr_t > ( driver.engine + signatures::dwClientState );
			std::uintptr_t UserInfoTable = driver.read< std::uintptr_t > ( ClientState + signatures::dwClientState_PlayerInfo );
			std::uintptr_t x = driver.read< std::uintptr_t > ( driver.read< std::uintptr_t > ( UserInfoTable + 0x40 ) + 0xC );
			PlayerInfo_t p = driver.read< PlayerInfo_t > ( driver.read< uintptr_t > ( x + 0x28 + 0x34 * Index ) );

			return p.name;
		}

		auto calculate_2d_distance ( geo::vec2_t src, geo::vec2_t dst ) -> float
		{
			return std::sqrt ( std::powf ( src.x - dst.x, 2 ) + std::powf ( src.y - dst.y, 2 ) );
		}

		auto line ( ImVec2 pos, ImVec2 size, ImU32 color, float thickness, ImDrawList* gui ) -> void
		{
			gui->AddLine ( pos, size, color, thickness );
		}

		auto stroke_text ( int x, int y, ImColor color, std::string str, ImDrawList* gui ) -> void
		{
			gui->AddText ( ImVec2 ( x, y - 1 ), ImGui::ColorConvertFloat4ToU32 ( ImVec4 ( 1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0 ) ), str.c_str ( ) );
			gui->AddText ( ImVec2 ( x, y + 1 ), ImGui::ColorConvertFloat4ToU32 ( ImVec4 ( 1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0 ) ), str.c_str ( ) );
			gui->AddText ( ImVec2 ( x - 1, y ), ImGui::ColorConvertFloat4ToU32 ( ImVec4 ( 1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0 ) ), str.c_str ( ) );
			gui->AddText ( ImVec2 ( x + 1, y ), ImGui::ColorConvertFloat4ToU32 ( ImVec4 ( 1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0 ) ), str.c_str ( ) );
			gui->AddText ( ImVec2 ( x, y ), color, str.c_str ( ) );
		}

		auto draw_rect ( float x, float y, float w, float h, ImColor color, ImDrawList* gui ) -> void
		{
			gui->AddLine ( { x, y }, { x + w, y }, color );
			gui->AddLine ( { x, y }, { x, y + h }, color );
			gui->AddLine ( { x + w, y }, { x + w, y + h }, color );
			gui->AddLine ( { x, y + h }, { x + w + 1, y + h }, color );
		}
	}

	std::uintptr_t local_player;
	std::uintptr_t local_player_team;
	char custom_name[128];
	bool third_person = false;

	auto get_local_values ( ) -> void
	{
		while ( true )
		{
			std::this_thread::sleep_for ( std::chrono::milliseconds ( 50 ) );

			local_player = driver.read<std::uintptr_t> ( driver.client + signatures::dwLocalPlayer );
			local_player_team = driver.read<std::uintptr_t> ( local_player + netvars::m_iTeamNum );
		}
	}

	auto run_local ( ) -> void
	{
		while ( true )
		{
			if ( !local_player ) continue;

			if ( globals::localplayer::bhop )
			{
				auto on_ground = driver.read<bool> ( local_player + netvars::m_fFlags );

				if ( GetAsyncKeyState ( VK_SPACE ) && on_ground & ( 1 << 0 ) )
					driver.write<BYTE> ( driver.client + signatures::dwForceJump, 6 );
			}

			if ( globals::localplayer::no_flash )
				driver.write<std::uintptr_t> ( local_player + netvars::m_flFlashMaxAlpha, 0 );
			else					
				driver.write<std::uintptr_t> ( local_player + netvars::m_flFlashMaxAlpha, 1132396544 );

			if ( globals::localplayer::skin_changer )
			{
				auto weapons = driver.read<std::array<unsigned long, 8>> ( local_player + netvars::m_hMyWeapons );

				for ( auto& handle : weapons )
				{
					auto weapon = driver.read<std::uintptr_t> ( ( driver.client + signatures::dwEntityList + ( handle & 0xFFF ) * 0x10 ) - 0x10 );

					if ( !weapon ) continue;

					if ( auto paint = functions::get_weapon_paint ( driver.read<short> ( weapon + netvars::m_iItemDefinitionIndex ) ) )
					{
						bool should_update = driver.read<std::int32_t> ( weapon + netvars::m_nFallbackPaintKit ) != paint;

						driver.write<std::int32_t> ( weapon + netvars::m_iItemIDHigh, -1 );

						driver.write<std::int32_t> ( weapon + netvars::m_nFallbackPaintKit, paint );
						driver.write<float> ( weapon + netvars::m_flFallbackWear, 0.1f );

						driver.write<std::string> ( weapon + netvars::m_szCustomName, custom_name );

						driver.write<int> ( weapon + netvars::m_iEntityQuality, 1 );

						if ( should_update )
						{
							//{
							//	int default_t = GetModelIndex ( "models/weapons/v_knife_default_t.mdl" );
							//	int default_ct = GetModelIndex ( "models/weapons/v_knife_default_ct.mdl" );
							//	int iBayonet = GetModelIndex ( "models/weapons/v_knife_bayonet.mdl" );
							//	int iFlip = GetModelIndex ( "models/weapons/v_knife_flip.mdl" );
							//	int iGut = GetModelIndex ( "models/weapons/v_knife_gut.mdl" );
							//	int iKarambit = GetModelIndex ( "models/weapons/v_knife_karam.mdl" );
							//	int iM9Bayonet = GetModelIndex ( "models/weapons/v_knife_m9_bay.mdl" );
							//	int iHuntsman = GetModelIndex ( "models/weapons/v_knife_tactical.mdl" );
							//	int iFalchion = GetModelIndex ( "models/weapons/v_knife_falchion_advanced.mdl" );
							//	int bowie = GetModelIndex ( "models/weapons/v_knife_survival_bowie.mdl" );
							//	int iButterfly = GetModelIndex ( "models/weapons/v_knife_butterfly.mdl" );
							//	int iDagger = GetModelIndex ( "models/weapons/v_knife_push.mdl" );

							//	driver.write<int> ( netvars::m_iItemDefinitionIndex, WEAPON_KNIFE_KARAMBIT );
							//	driver.write<int> ( netvars::m_nModelIndex, iKarambit );
							//	driver.write<int> ( netvars::m_nFallbackPaintKit, 409 );
							//	driver.write<int> ( 0x2FAC, 3 );
							//	driver.write<int> ( netvars::m_iItemIDHigh, 1 );
							//	driver.write<int> ( netvars::m_nModelIndex, iKarambit );
							//}
							//DWORD _dwClientState = driver.read<DWORD> ( driver.engine + signatures::dwClientState ); driver.write<int> ( _dwClientState + 0x174, -1 );

							driver.write<std::int32_t> ( driver.read<std::uintptr_t> ( driver.engine + signatures::dwClientState ) + 0x174, -1 );
						}
					}
				}
			}

			std::this_thread::sleep_for ( std::chrono::milliseconds ( 1 ) );
		}
	}

	int aim_bone{ 0 };
	const char* bone_string[ 2 ]
	{
		"Head",
		"Spine"
	};

	auto run_aimbot ( ) -> void
	{
		while ( true )
		{
			std::this_thread::sleep_for ( std::chrono::milliseconds ( 1 ) );

			if ( !local_player ) continue;

			auto aim_punch = driver.read<geo::vec3_t> ( local_player + netvars::m_aimPunchAngle ) * 2;
			auto view_matrix = driver.read< view_matrix_t > ( driver.client + signatures::dwViewMatrix );
			auto client_state = driver.read<std::uintptr_t> ( driver.engine + signatures::dwClientState );
			auto view_angles = driver.read<geo::vec3_t> ( client_state + signatures::dwClientState_ViewAngles );
			auto local_eye_pos = driver.read<geo::vec3_t> ( local_player + netvars::m_vecOrigin ) + driver.read<geo::vec3_t> ( local_player + netvars::m_vecViewOffset );
			for ( int i = 0; i <= 32; i++ )
			{
				if ( !GetAsyncKeyState ( globals::aimbot::bind ) )
					continue;

				auto entity = driver.read<std::uintptr_t> ( driver.client + signatures::dwEntityList + ( i * 0x10 ) );
				if ( !entity ) continue;

				auto dormant = driver.read<bool> ( entity + signatures::m_bDormant );
				if ( dormant ) continue;

				auto entity_hp = driver.read<int> ( entity + netvars::m_iHealth );
				if ( !entity_hp ) continue;

				auto entity_team = driver.read<std::uintptr_t> ( entity + netvars::m_iTeamNum );
				if ( entity_team == local_player_team ) continue;

				geo::vec3_t aim_pos;

				if ( aim_bone == 0 )
					aim_pos = functions::get_bone_pos ( entity, player_bones::head );
				else if (aim_bone = 1)
					aim_pos = functions::get_bone_pos ( entity, player_bones::m_spine );

				geo::vec2_t screen_head_pos;
				if ( !functions::world_to_screen ( aim_pos, screen_head_pos, view_matrix ) ) continue;
			
				if ( !globals::aimbot::x360 )
				{
					if ( ( functions::calculate_2d_distance ( geo::vec2_t ( globals::width / 2, globals::height / 2 ), screen_head_pos ) - 4.00f ) > globals::aimbot::aim_fov ) continue;
				}

				auto angle = functions::calculate_angle
				(
					local_eye_pos,
					aim_pos,
					view_angles + aim_punch
				);

				driver.write<geo::vec3_t> ( client_state + signatures::dwClientState_ViewAngles, view_angles + angle / globals::aimbot::smoothing );
			}
		}
	}

	auto run_esp ( ImDrawList* gui ) -> void
	{
		if ( local_player )
		{
			for ( int i = 0; i <= 32; i++ )
			{
				auto view_matrix = driver.read< view_matrix_t > ( driver.client + signatures::dwViewMatrix );

				auto entity = driver.read<std::uintptr_t> ( driver.client + signatures::dwEntityList + ( i * 0x10 ) );
				if ( !entity ) continue;

				auto entity_hp = driver.read<int> ( entity + netvars::m_iHealth );
				if ( !entity_hp ) continue;

				if ( entity == local_player ) continue;

				auto local_hp = driver.read<int> ( local_player + netvars::m_iHealth );
				auto entity_team = driver.read<std::uintptr_t> ( entity + netvars::m_iTeamNum );
				auto entity_pos = driver.read<geo::vec3_t> ( entity + netvars::m_vecOrigin );
				auto entity_flags = driver.read<int> ( entity + netvars::m_fFlags );
				auto entity_head_pos = functions::get_bone_pos ( entity, 8 );

				geo::vec3_t top_player = { entity_pos.x, entity_pos.y, ( entity_flags & ducking ) ? entity_pos.z + 60.f : entity_pos.z + 76.f };

				auto w = 4.00f;
				geo::vec2_t top, bottom;

				if ( !functions::world_to_screen ( top_player, top, view_matrix ) || !functions::world_to_screen ( entity_pos, bottom, view_matrix ) ) continue;

				float box_height = bottom.y - top.y;
				float box_width = box_height / 2;

				if ( globals::visuals::box )
				{
					if ( entity_team == local_player_team )
						functions::draw_rect ( top.x - box_width / 2, top.y + 5.f, box_width, box_height, ImColor ( 255, 255, 255 ), gui );
					else
						functions::draw_rect ( top.x - box_width / 2, top.y + 5.f, box_width, box_height, ImColor ( 255, 0, 0 ), gui );
				}
				if ( globals::visuals::name )
				{
					functions::stroke_text ( top.x + ( box_width / 2.00f ) + 15.00f, top.y + 2.50f, ImColor ( 255, 255, 255 ), functions::display_name ( i ).c_str ( ), gui );
				}
				if ( globals::visuals::health )
				{
					functions::stroke_text ( bottom.x + ( box_width / 2 ) + 16, bottom.y - 5.00f, ImColor ( 255, 255, 255 ), std::to_string ( entity_hp ).c_str ( ), gui );

					float health_pos = ( box_height - ( box_height * entity_hp / 100.f ) );
					float box = std::ceil ( entity_hp / 12.00f );
					float multiplier = 12.00f / 360.f; multiplier *= box - 1;
					c_color col_health = c_color::from_hsb ( multiplier, 1, 1 );

					gui->AddRectFilled ( { bottom.x + box_height / w + 10, top.y + 6.00f }, { bottom.x + box_height / w + 8, bottom.y + 6.00f }, ImColor ( 0, 0, 0 ) );

					gui->AddRectFilled ( { bottom.x + box_height / w + 10, top.y + health_pos + 6.00f }, { bottom.x + box_height / w + 8, bottom.y + 6.00f }, ImColor ( col_health.r, col_health.g, col_health.b, col_health.a ) );
				}
				if ( globals::visuals::glow )
				{
					auto glow_manager = driver.read<std::uintptr_t> ( driver.client + signatures::dwGlowObjectManager );
					auto glow_index = driver.read<std::int32_t> ( entity + netvars::m_iGlowIndex );
					if ( entity_team == local_player_team )
					{
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x8, 1.0f ); // red
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0xC, 1.0f ); // green
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x10, 1.0f ); // blue
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x14, 0.8f ); // Brightness
					}
					else
					{
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x8, 1.0f ); // red
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0xC, 0.0f ); // green
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x10, 0.0f ); // blue
						driver.write<float> ( glow_manager + ( glow_index * 0x38 ) + 0x14, 0.8f ); // Brightness
					}

					driver.write<bool> ( glow_manager + ( glow_index * 0x38 ) + 0x27, true );
					driver.write<bool> ( glow_manager + ( glow_index * 0x38 ) + 0x28, true );
				}
				if ( globals::visuals::tracers )
				{
					if ( entity_team == local_player_team )
						functions::line ( { ( float )globals::width / 2, ( float )globals::height }, { bottom.x, bottom.y }, ImColor ( 255, 255, 255 ), 1.00f, gui );
					else
						functions::line ( { ( float )globals::width / 2, ( float )globals::height }, { bottom.x, bottom.y }, ImColor ( 255, 0, 0 ), 1.00f, gui );
				}
			}
		}
	}
}

IDirect3DTexture9* byteImage = nullptr;
int skin_type{ 0 };
bool skin_selected[ 44 ];
int currentColumn{ 0 };
const char* skin_string[ 6 ]
{
	"Ak47",
	"M4a4",
	"M4a1s",
	"Usp-s",
	"Awupa",
	"Glock"
};

auto read_file ( std::string file ) -> std::string
{
	std::string line;
	std::ifstream myfile ( file );
	if ( myfile.is_open ( ) )
	{
		while ( getline ( myfile, line ) )
		{
			return line;
		}
		myfile.close ( );
	}
}

auto remove_spaces ( std::string str ) -> std::string
{
	str.erase ( remove ( str.begin ( ), str.end ( ), ' ' ), str.end ( ) );
	return str;
}

std::string file_pass = read_file ( "C:\\rainycheats\\login.json" );
std::string fileusername = remove_spaces ( file_pass.substr ( file_pass.find ( ">" ) + 1 ) );

auto render ( ) -> void
{
	if ( GetAsyncKeyState ( VK_INSERT ) & 1 ) showing = !showing;
	ImGui_ImplDX9_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );
	ImGui::GetIO ( ).MouseDrawCursor = showing;

	geo::vec2_t screen_center = { ( float )sdk::globals::width / 2, ( float )sdk::globals::height / 2 };
	auto background = ImGui::GetBackgroundDrawList ( );
	auto foreground = ImGui::GetForegroundDrawList ( );
	auto gui = background;

	std::vector<std::string> listthingy;
	if ( showing )
	{
		//if ( !byteImage )
		//	D3DXCreateTextureFromFileA ( directx::pDevice, "C:\\rainycheats\\avatar.jpg", &byteImage );

		ImGui::SetNextWindowSize ( ImVec2 ( 550, 440 ) );
		ImGui::Begin ( "rainy", nullptr, ImGuiWindowFlags_NoDecoration );
		{
			auto draw = ImGui::GetWindowDrawList ( );
			ImVec2 pos = ImGui::GetWindowPos ( );
			ImVec2 size = ImGui::GetWindowSize ( );

			draw->AddRectFilled ( pos, ImVec2 ( pos.x + size.x, pos.y + 45 ), ImColor ( 18, 16, 40 ), 2.0f, ImDrawFlags_RoundCornersTop );
			draw->AddRectFilled ( ImVec2 ( pos.x, pos.y + 45 ), ImVec2 ( pos.x + size.x, pos.y + 47 ), ImColor ( 60, 57, 101, 180 ) );

			draw->AddText ( ImVec2 ( pos.x + 10, pos.y + 16 ), ImColor ( 1.0f, 1.0f, 1.0f ), "rainycheats.xyz" );

			draw->AddImageRounded ( byteImage, ImVec2 ( pos.x + size.x - 30, pos.y + 13 ), ImVec2 ( pos.x + size.x - 10, pos.y + 33 ), ImVec2 ( 0, 0 ), ImVec2 ( 1, 1 ), ImColor ( 1.0f, 1.0f, 1.0f ), 20.0f );
			draw->AddText ( ImVec2 ( pos.x + size.x - 32 - ImGui::CalcTextSize ( fileusername.c_str ( ) ).x, pos.y + 16 ), ImColor ( 1.0f, 1.0f, 1.0f ), fileusername.c_str ( ) );

			ImGui::SetCursorPos ( ImVec2 ( 190, 11 ) );
			ImGui::BeginGroup ( );
			if ( imgui::tab ( "aim", 0 == currentColumn ) ) currentColumn = 0;
			ImGui::SameLine ( );
			if ( imgui::tab ( "visuals", 1 == currentColumn ) ) currentColumn = 1;
			ImGui::SameLine ( );
			if ( imgui::tab ( "misc", 2 == currentColumn ) ) currentColumn = 2;
			ImGui::SameLine ( );
			if ( imgui::tab ( "config", 3 == currentColumn ) ) currentColumn = 3;
			ImGui::EndGroup ( );

			ImGui::SetCursorPos ( ImVec2 ( 100, 11 ) );
			if ( imgui::tab ( "exit", 0 ) )
			{
				Sleep ( 150 );
				exit ( 0 );
			}

			switch ( currentColumn ) 
			{
			case 0:
				ImGui::SetCursorPos ( ImVec2 ( 20, 65 ) );
				ImGui::BeginChild ( "Aimbot", ImVec2 ( 510, 355 ) );
				{
					ImGui::BeginGroup ( );
					{
						ImGui::SetCursorPos ( ImVec2 ( 5, NULL ) );
						ImGui::Checkbox ( "aimbot", &sdk::globals::aimbot::enabled );
						if ( sdk::globals::aimbot::enabled )
						{
							ImGui::SameLine ( );
							imgui::hot_key ( "aimbot bind", &sdk::globals::aimbot::bind );
							ImGui::SetCursorPos ( ImVec2 ( 5, 20 ) );
							ImGui::SliderFloat ( "smoothing", &sdk::globals::aimbot::smoothing, 1.00f, 100.00f, "%.1f" );
							ImGui::SetCursorPos ( ImVec2 ( 5, 60 ) );
							ImGui::Checkbox ( "360 aimbot", &sdk::globals::aimbot::x360 );
							ImGui::SetCursorPos ( ImVec2 ( 5, 80 ) );
							ImGui::SliderFloat ( "fov size", &sdk::globals::aimbot::aim_fov, 0.00f, 1000.00f, "%.1f" );
							ImGui::SetCursorPos ( ImVec2 ( 5, 120 ) );
							ImGui::Combo ( "aim bone", &sdk::aim_bone, sdk::bone_string, 2 );
						}
					}
					ImGui::EndGroup ( );
				}
				ImGui::EndChild ( );
				break;
			case 1:
				ImGui::SetCursorPos ( ImVec2 ( 20, 65 ) );
				ImGui::BeginChild ( "Visuals", ImVec2 ( 510, 355 ) );
				{
					ImGui::BeginGroup ( );
					{
						ImGui::SetCursorPos ( ImVec2 ( 5, NULL ) );
						ImGui::Checkbox ( "esp", &sdk::globals::visuals::esp );
						if ( sdk::globals::visuals::esp )
						{
							ImGui::SetCursorPos ( ImVec2 ( 5, 20 ) );
							ImGui::Checkbox ( "box", &sdk::globals::visuals::box );
							ImGui::SetCursorPos ( ImVec2 ( 5, 40 ) );
							ImGui::Checkbox ( "glow", &sdk::globals::visuals::glow );
							ImGui::SetCursorPos ( ImVec2 ( 5, 60 ) );
							ImGui::Checkbox ( "name", &sdk::globals::visuals::name );
							ImGui::SetCursorPos ( ImVec2 ( 5, 80 ) );
							ImGui::Checkbox ( "health", &sdk::globals::visuals::health );
							ImGui::SetCursorPos ( ImVec2 ( 5, 100 ) );
							ImGui::Checkbox ( "tracers", &sdk::globals::visuals::tracers );
						}
						//ImGui::SetCursorPos ( ImVec2 ( 5, 120 ) );
						//ImGui::SliderFloat ( "local fov", &sdk::globals::localplayer::local_fov, 60.00f, 100.00f, "%.1f" );
					}
					ImGui::EndGroup ( );
				}
				ImGui::EndChild ( );
				break;
			case 2:
				ImGui::SetCursorPos ( ImVec2 ( 20, 65 ) );
				ImGui::BeginChild ( "Misc", ImVec2 ( 510, 355 ) );
				{
					ImGui::BeginGroup ( );
					{
						ImGui::SetCursorPos ( ImVec2 ( 5, NULL ) );
						ImGui::Checkbox ( "bhop", &sdk::globals::localplayer::bhop );
						ImGui::SetCursorPos ( ImVec2 ( 5, 20 ) );
						ImGui::Checkbox ( "no flash", &sdk::globals::localplayer::no_flash );
						ImGui::SetCursorPos ( ImVec2 ( 5, 40 ) );
						ImGui::Checkbox ( "skin changer", &sdk::globals::localplayer::skin_changer );
						if ( skin_type == 0 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::ak47_names ); i++ )
							{
								listthingy.push_back ( sdk::ak47_names[ i ] );
							}
						}
						else if ( skin_type == 1 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::m4a4_skins ); i++ )
							{
								listthingy.push_back ( sdk::m4a4_skins[ i ] );
							}
						}
						else if ( skin_type == 2 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::m4a1s_skins ); i++ )
							{
								listthingy.push_back ( sdk::m4a1s_skins[ i ] );
							}
						}
						else if ( skin_type == 3 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::usps_skins ); i++ )
							{
								listthingy.push_back ( sdk::usps_skins[ i ] );
							}
						}
						else if ( skin_type == 4 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::awupa_names ); i++ )
							{
								listthingy.push_back ( sdk::awupa_names[ i ] );
							}
						}
						else if ( skin_type == 5 )
						{
							listthingy.clear ( );
							for ( size_t i = 0; i < IM_ARRAYSIZE ( sdk::glock_names ); i++ )
							{
								listthingy.push_back ( sdk::glock_names[ i ] );
							}
						}

						if ( sdk::globals::localplayer::skin_changer )
						{
							ImGui::SetNextItemWidth ( 200.00f );
							ImGui::SetCursorPos ( ImVec2 ( 5, 60 ) );
							ImGui::Combo ( "weapon", &skin_type, skin_string, 6 );

							static std::string preview = "";
							static ImGuiTextFilter filter;
							static char tag_buffer[ 256 ];

							static int changed_skin;

							
						}
					}
					ImGui::EndGroup ( );
				}
				ImGui::EndChild ( );
				break;
			case 3:
				ImGui::SetCursorPos ( ImVec2 ( 20, 65 ) );
				ImGui::BeginChild ( "Config", ImVec2 ( 510, 355 ) );
				{
					ImGui::BeginGroup ( );
					{

					}
					ImGui::EndGroup ( );
				}	
				ImGui::EndChild ( );
				break;
			}
		}
		ImGui::End ( );
		SetWindowLong ( overlay_window::Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW );
		UpdateWindow ( overlay_window::Hwnd );
		SetFocus ( overlay_window::Hwnd );
	}
	else
	{
		listthingy.clear ( );
		SetWindowLong ( overlay_window::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );
		UpdateWindow ( overlay_window::Hwnd );
	}
	if ( sdk::globals::visuals::esp ) sdk::run_esp ( background );
	if ( sdk::globals::aimbot::enabled && !sdk::globals::aimbot::x360 ) gui->AddCircle ( { screen_center.x, screen_center.y }, sdk::globals::aimbot::aim_fov, ImColor ( 255, 255, 255 ) );
	ImGui::EndFrame ( );

	directx::pDevice->SetRenderState ( D3DRS_ZENABLE, false );
	directx::pDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, false );
	directx::pDevice->SetRenderState ( D3DRS_SCISSORTESTENABLE, false );

	directx::pDevice->Clear ( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB ( 0, 0, 0, 0 ), 1.0f, 0 );
	if ( directx::pDevice->BeginScene ( ) >= 0 )
	{
		ImGui::Render ( );
		ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );
		directx::pDevice->EndScene ( );
	}

	HRESULT result = directx::pDevice->Present ( NULL, NULL, NULL, NULL );
	if ( result == D3DERR_DEVICELOST && directx::pDevice->TestCooperativeLevel ( ) == D3DERR_DEVICENOTRESET )
	{
		ImGui_ImplDX9_InvalidateDeviceObjects ( );
		directx::pDevice->Reset ( &directx::pParams );
		ImGui_ImplDX9_CreateDeviceObjects ( );
	}
}

auto main_loop ( ) -> void
{
	static RECT OldRect;
	ZeroMemory ( &directx::Message, sizeof ( MSG ) );

	while ( directx::Message.message != WM_QUIT )
	{
		if ( PeekMessage ( &directx::Message, overlay_window::Hwnd, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage ( &directx::Message );
			DispatchMessage ( &directx::Message );
		}
		HWND ForegroundWindow = GetForegroundWindow ( );
		if ( ForegroundWindow == process::Hwnd )
		{
			HWND TempProcessHwnd = GetWindow ( ForegroundWindow, GW_HWNDPREV );
			SetWindowPos ( overlay_window::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		}

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory ( &TempRect, sizeof ( RECT ) );
		ZeroMemory ( &TempPoint, sizeof ( POINT ) );

		GetClientRect ( process::Hwnd, &TempRect );
		ClientToScreen ( process::Hwnd, &TempPoint );

		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO ( );
		io.ImeWindowHandle = process::Hwnd;

		POINT TempPoint2;
		GetCursorPos ( &TempPoint2 );
		io.MousePos.x = TempPoint2.x - TempPoint.x;
		io.MousePos.y = TempPoint2.y - TempPoint.y;

		if ( GetAsyncKeyState ( 0x1 ) )
		{
			io.MouseDown[ 0 ] = true;
			io.MouseClicked[ 0 ] = true;
			io.MouseClickedPos[ 0 ].x = io.MousePos.x;
			io.MouseClickedPos[ 0 ].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[ 0 ] = false;
		}

		if ( TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom )
		{
			OldRect = TempRect;
			process::WindowWidth = TempRect.right;
			process::WindowHeight = TempRect.bottom;
			directx::pParams.BackBufferWidth = process::WindowWidth;
			directx::pParams.BackBufferHeight = process::WindowHeight;
			SetWindowPos ( overlay_window::Hwnd, ( HWND )0, TempPoint.x, TempPoint.y, process::WindowWidth, process::WindowHeight, SWP_NOREDRAW );
			directx::pDevice->Reset ( &directx::pParams );
		}
		render ( );
	}
	ImGui_ImplDX9_Shutdown ( );
	ImGui_ImplWin32_Shutdown ( );
	ImGui::DestroyContext ( );
	if ( directx::pDevice != NULL )
	{
		directx::pDevice->EndScene ( );
		directx::pDevice->Release ( );
	}
	if ( directx::Direct3D9 != NULL )
	{
		directx::Direct3D9->Release ( );
	}
	DestroyWindow ( overlay_window::Hwnd );
	UnregisterClass ( overlay_window::WindowClass.lpszClassName, overlay_window::WindowClass.hInstance );
}

auto directx_init ( ) -> bool
{
	if ( FAILED ( Direct3DCreate9Ex ( D3D_SDK_VERSION, &directx::Direct3D9 ) ) )
	{
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = overlay_window::Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = process::WindowWidth;
	Params.BackBufferHeight = process::WindowHeight;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if ( FAILED ( directx::Direct3D9->CreateDeviceEx ( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlay_window::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &directx::pDevice ) ) )
	{
		directx::Direct3D9->Release ( );
		return false;
	}

	ImGui::CreateContext ( );
	ImGuiIO& io = ImGui::GetIO ( ); ( void )io;
	ImGuiStyle* style = &ImGui::GetStyle ( );
	ImFontConfig font_config;
	io.IniFilename = nullptr;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[ ] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0, 0x2DFF, // Cyrillic Extended-A
		0xA640, 0xA69F, // Cyrillic Extended-B
		0xE000, 0xE226, // icons
		0,
	};

	style->Colors[ ImGuiCol_WindowBg ] = ImColor ( 16, 12, 29 );
	style->Colors[ ImGuiCol_Button ] = ImColor ( 29, 31, 39 );
	style->Colors[ ImGuiCol_ButtonActive ] = ImColor ( 48, 48, 47 );
	style->Colors[ ImGuiCol_ButtonHovered ] = ImColor ( 38, 38, 37 );
	style->Colors[ ImGuiCol_FrameBgActive ] = ImColor ( 80, 15, 174 );
	style->Colors[ ImGuiCol_FrameBg ] = ImColor ( 10, 10, 10 );
	style->Colors[ ImGuiCol_FrameBgHovered ] = ImColor ( 90, 25, 184 );

	font_config.GlyphRanges = ranges;

	io.DeltaTime = 1.f;
	io.Fonts->AddFontFromFileTTF ( "C:\\Windows\\Fonts\\verdana.ttf", 13.0f, &font_config, ranges );

	ImGui_ImplWin32_Init ( overlay_window::Hwnd );
	ImGui_ImplDX9_Init ( directx::pDevice );
	directx::Direct3D9->Release ( );
	return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK WinProc ( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	if ( ImGui_ImplWin32_WndProcHandler ( hWnd, Message, wParam, lParam ) )
		return true;

	switch ( Message ) {

	case WM_DESTROY:
		if ( directx::pDevice != NULL )
		{
			directx::pDevice->EndScene ( );
			directx::pDevice->Release ( );
		}
		if ( directx::Direct3D9 != NULL )
		{
			directx::Direct3D9->Release ( );
		}
		PostQuitMessage ( 0 );
		exit ( 4 );
		break;
	case WM_SIZE:
		if ( directx::pDevice != NULL && wParam != SIZE_MINIMIZED )
		{
			ImGui_ImplDX9_InvalidateDeviceObjects ( );
			directx::pParams.BackBufferWidth = LOWORD ( lParam );
			directx::pParams.BackBufferHeight = HIWORD ( lParam );
			HRESULT hr = directx::pDevice->Reset ( &directx::pParams );
			if ( hr == D3DERR_INVALIDCALL )
				IM_ASSERT ( 0 );
			ImGui_ImplDX9_CreateDeviceObjects ( );
		}
		break;
	default:
		return DefWindowProc ( hWnd, Message, wParam, lParam );
		break;
	}
	return 0;
}

auto setup_window ( ) -> void
{
	overlay_window::WindowClass =
	{
		sizeof ( WNDCLASSEX ), 0, WinProc, 0, 0, nullptr, LoadIcon ( nullptr, IDI_APPLICATION ), LoadCursor ( nullptr, IDC_ARROW ), nullptr, nullptr, overlay_window::Name, LoadIcon ( nullptr, IDI_APPLICATION )
	};

	RegisterClassEx ( &overlay_window::WindowClass );
	if ( process::Hwnd )
	{
		static RECT TempRect = { NULL };
		static POINT TempPoint;
		GetClientRect ( process::Hwnd, &TempRect );
		ClientToScreen ( process::Hwnd, &TempPoint );
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		process::WindowWidth = TempRect.right;
		process::WindowHeight = TempRect.bottom;
	}

	overlay_window::Hwnd = CreateWindowEx ( NULL, overlay_window::Name, overlay_window::Name, WS_POPUP | WS_VISIBLE, process::WindowLeft, process::WindowTop, process::WindowWidth, process::WindowHeight, NULL, NULL, 0, NULL );
	DwmExtendFrameIntoClientArea ( overlay_window::Hwnd, &directx::Margin );
	SetWindowLong ( overlay_window::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );
	ShowWindow ( overlay_window::Hwnd, SW_SHOW );
	UpdateWindow ( overlay_window::Hwnd );
}

auto start_overlay ( ) -> void
{
	bool window_focus = false;
	while ( window_focus == false )
	{
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId ( GetForegroundWindow ( ), &ForegroundWindowProcessID );
		if ( utils::get_process_id ( "csgo.exe" ) == ForegroundWindowProcessID )
		{
			process::ID = GetCurrentProcessId ( );
			process::Handle = GetCurrentProcess ( );
			process::Hwnd = GetForegroundWindow ( );

			RECT TempRect;
			GetWindowRect ( process::Hwnd, &TempRect );
			process::WindowWidth = TempRect.right - TempRect.left;
			process::WindowHeight = TempRect.bottom - TempRect.top;
			process::WindowLeft = TempRect.left;
			process::WindowRight = TempRect.right;
			process::WindowTop = TempRect.top;
			process::WindowBottom = TempRect.bottom;

			char TempTitle[ MAX_PATH ];
			GetWindowText ( process::Hwnd, TempTitle, sizeof ( TempTitle ) );
			process::Title = TempTitle;

			char TempClassName[ MAX_PATH ];
			GetClassName ( process::Hwnd, TempClassName, sizeof ( TempClassName ) );
			process::ClassName = TempClassName;

			char TempPath[ MAX_PATH ];
			GetModuleFileNameEx ( process::Handle, NULL, TempPath, sizeof ( TempPath ) );
			process::Path = TempPath;

			window_focus = true;
		}
		if ( !utils::get_process_id ( "csgo.exe" ) ) exit ( 0 );
	}

	overlay_window::Name = utils::random_string ( 10 ).c_str ( );
	setup_window ( );
	directx_init ( );
	main_loop ( );
}

int main ( )
{
	ShowWindow ( GetConsoleWindow ( ), SW_HIDE );
	if ( driver.init ( ) )
	{
		driver.client = utils::get_module ( utils::globals::process_id, L"client.dll" );
		driver.engine = utils::get_module ( utils::globals::process_id, L"engine.dll" );

		std::thread ( [ & ]( ) { start_overlay ( ); } ).detach ( );
		std::thread ( [ & ]( ) { sdk::get_local_values ( ); } ).detach ( );
		Sleep ( 500 );
		std::thread ( [ & ]( ) { sdk::run_local ( ); } ).detach ( );
		Sleep ( 500 );
		std::thread ( [ & ]( ) { sdk::run_aimbot ( ); } ).detach ( );
	}
	else
	{
		std::cout << "game not open" << std::endl;
	}
	Sleep ( -1 );
}