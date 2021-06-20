/**
 * @file error.cpp
 *
 * Implementation of in-game message functions.
 */
#include "error.h"

#include "control.h"
#include "engine/render/cel_render.hpp"
#include "engine/render/text_render.hpp"
#include "stores.h"
#include "utils/language.h"

namespace devilution {

diablo_message msgtable[MAX_SEND_STR_LEN];
DWORD msgdelay;
diablo_message msgflag;
uint8_t msgcnt;

/** Maps from error_id to error message. */
const char *const MsgStrings[] = {
	"",
	N_("B FOPODE KAPTA HEDOCTYZHA"),
	N_("B DEMO BEPCNN MY/LTNZ/EEP HEDOCTYZEH"),
	N_("OUN#KA BLIBODA 3BYKA"),
	N_("HEDOCTYZHO B GTO& BEPCNN"),
	N_("HEDOCTATO4HO MECTA, 4TO#LI COXPAHNTLCR"),
	N_("B FOPODE ZAY3A HE PA#OTAET"),
	N_("PEKOMEHDYETCR CKOZNPOBATL NFPY HA )KSCTKNN DNCK"),
	N_("ZPO#/EMA B CNHXPOHN3AQNN"),
	N_("B MY/LTNZ/EEPE ZAY3A HE PA#OTAET"),
	N_("3AFPY3KA..."),
	N_("COXPAHEHNE..."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HEKOTOPLIE OC/A#HYT, 4TO#LI ODNH YCN/N/CR"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HOBAR CN/A KYETCR 4EPE3 PA3PYUEHNE"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "TOT KTO 3AVNVAETCR, PEDKO HAZADAET"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "ME4 ZPABOCYDNR #LICTP N OCTP"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "ZOKA DYX #ODPCTBYET TE/O PACQBETAET"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "MOVL MAFNN HAZPABNTCR TYDA FDE ES HET"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BPEMR HE ZODB/ACTHO KPEZKOCTN CTA/N"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "MAFNR HE BCEFDA TO, 4EM KA)KETCR"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "4TO #LI/O OTKPLITLIM, 3AKPLITO TEZEPL"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "COCPEDOTO4NE ZPNXODNT QEHO& MYDPOCTN"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "TA&HOE 3HAHNE HECST PA3PYUEHNE"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "TO, 4TO HE/L3R YDEP)KATL, HE/L3R ZOBPEDNTL"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "KAPMNH N /A3YPL CTAHYT KAK CO/HQE"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "3HAHNE N MYDPOCTL - QEHA CAMOFO CE#R"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BLIZE& N OCBE)KNCL"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "KYDA NDSTE, TAM N ZPE#YDETE"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "GHEPFNR ZPNXODNT QEHO& MYDPOCTN"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "#OFATCBO ZPNXODNT HE)KDAHHO"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "FDE A/4HOCTL ZADET, TAM TEPZEHNE HA&DET HAFPADY"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "#/AFOC/OB/EHLI DO#PLIM TOBAPNVEM!"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BAUN PYKN ZPABRT BAUE& CYDL#O&"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "CN/Y YKPEZ/RET #O)KECTBEHHAR BEPA"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "CYTL )KN3HN NCXODNT N3HYTPN"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "CBEPXY BAU ZYTL CTAHET RCHE&"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "CZACEHNE ZPNXODNT QEHO& MYDPOCTN"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BCS MNCTN4ECKOE PACKPOETCR B CBETE BAUEFO PA3YMA"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "ZOC/EDHNE EVE MOFYT #LITL ZEPBLIMN"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HN ODHO DO#POE DE/O HE OCTAETCR #E3HAKA3AHHLIM"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HEO#XODNM ZO KPA&HE& MEPE 8 YPOBEHL."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HEO#XODNM ZO KPA&HE& MEPE 13 YPOBEHL."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "HEO#XODNM ZO KPA&HE& MEPE 17 YPOBEHL."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "ZO/Y4EHLI TA&HLIE 3HAHNR!"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "TO, 4TO HE Y#NBAET TE#R..."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "3HAHNE - CN/A."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "DAWVEMY, DA BO3DACTCR!"),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "NHOFDA HY)KHO ZOTPOFATL, 4TO#LI ZOHRTL."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "B FOCTRX XOPOUO, A DOMA /Y4UE."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "Spiritual energy is restored."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BLI 4YBCTBYETE CE#R #O/EE /OBKNM."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BLI 4YBCTBYETE CE#R CN/LHEE."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BLI 4YBCTBYETE CE#R MYDPEE."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "BLI 4YBCTBYETE CE#R OCBE)KSHHLIM."),
	N_(/* TRANSLATORS: Shrine Text. Keep atmospheric. :) */ "GTO MO)KET C/OMNTL BO/W."),
};

void InitDiabloMsg(diablo_message e)
{
	int i;

	if (msgcnt >= sizeof(msgtable))
		return;

	for (i = 0; i < msgcnt; i++) {
		if (msgtable[i] == e)
			return;
	}

	msgtable[msgcnt] = e; // BUGFIX: missing out-of-bounds check (fixed)
	msgcnt++;

	msgflag = msgtable[0];
	msgdelay = SDL_GetTicks();
}

void ClrDiabloMsg()
{
	for (auto &msg : msgtable)
		msg = EMSG_NONE;

	msgflag = EMSG_NONE;
	msgcnt = 0;
}

#define DIALOG_Y ((gnScreenHeight - PANEL_HEIGHT) / 2 - 18)

void DrawDiabloMsg(const CelOutputBuffer &out)
{
	CelDrawTo(out, { PANEL_X + 101, DIALOG_Y }, *pSTextSlidCels, 1);
	CelDrawTo(out, { PANEL_X + 527, DIALOG_Y }, *pSTextSlidCels, 4);
	CelDrawTo(out, { PANEL_X + 101, DIALOG_Y + 48 }, *pSTextSlidCels, 2);
	CelDrawTo(out, { PANEL_X + 527, DIALOG_Y + 48 }, *pSTextSlidCels, 3);

	int sx = PANEL_X + 109;
	for (int i = 0; i < 35; i++) {
		CelDrawTo(out, { sx, DIALOG_Y }, *pSTextSlidCels, 5);
		CelDrawTo(out, { sx, DIALOG_Y + 48 }, *pSTextSlidCels, 7);
		sx += 12;
	}
	int sy = DIALOG_Y + 12;
	for (int i = 0; i < 3; i++) {
		CelDrawTo(out, { PANEL_X + 101, sy }, *pSTextSlidCels, 6);
		CelDrawTo(out, { PANEL_X + 527, sy }, *pSTextSlidCels, 8);
		sy += 12;
	}

	DrawHalfTransparentRectTo(out, PANEL_X + 104, DIALOG_Y - 8, 432, 54);

	strcpy(tempstr, _(MsgStrings[msgflag]));
	DrawString(out, tempstr, { PANEL_X + 101, DIALOG_Y + 24, 442, 0 }, UIS_CENTER);

	if (msgdelay > 0 && msgdelay <= SDL_GetTicks() - 3500) {
		msgdelay = 0;
	}
	if (msgdelay == 0) {
		msgcnt--;
		if (msgcnt == 0) {
			msgflag = EMSG_NONE;
		} else {
			msgflag = msgtable[msgcnt];
			msgdelay = SDL_GetTicks();
		}
	}
}

} // namespace devilution
