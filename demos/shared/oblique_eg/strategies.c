/**
 * MIT License
 *
 * Copyright (c) 2021 Klatu Networks, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// =============================================================================
// Includes

#include "strategies.h"
#include "mu_platform.h"
#include "mulib.h"

// =============================================================================
// Local types and definitions

#define MAX_STRATEGY_LEN 130

// =============================================================================
// Local (forward) declarations

static char rot13(char c);

// =============================================================================
// Local storage

const static const char *s_strategies[] = {
    "(Betnavp) znpuvarel",
    "Abg ohvyqvat n jnyy ohg znxvat n oevpx",
    "Abg ohvyqvat n jnyy; znxvat n oevpx",
    "Bapr gur frnepu unf ortha, fbzrguvat jvyy or sbhaq",
    "Bapr gur frnepu vf va cebterff, fbzrguvat jvyy or sbhaq",
    "Bayl bar ryrzrag bs rnpu xvaq",
    "Bayl n cneg, abg gur jubyr",
    "Bcrayl erfvfg punatr",
    "Biregyl erfvfg punatr",
    "Chg va rnecyhtf",
    "Cnr Juvgr'f aba-oynax tencuvp zrgnpneq",
    "Dhrfgvba gur urebvp nccebnpu",
    "Dhrfgvba gur urebvp",
    "Ercrgvgvba vf n sbez bs punatr",
    "Ergenpr lbhe fgrcf",
    "Erinyhngvba (n jnez srryvat)",
    "Erirefr",
    "Erzbir fcrpvsvpf naq pbaireg gb nzovthvgvrf",
    "Erzbir n erfgevpgvba",
    "Erzbir nzovthvgvrf naq pbaireg gb fcrpvsvpf",
    "Erzrzore .gubfr dhvrg riravatf",
    "Erzrzore dhvrg riravatf",
    "Fcrpgehz nanylfvf",
    "Fgngr gur ceboyrz nf pyrneyl nf cbffvoyr",
    "Fgngr gur ceboyrz va jbeqf nf fvzcyl nf cbffvoyr",
    "Fgngr gur ceboyrz va jbeqf nf pyrneyl nf cbffvoyr",
    "Fubeg pvephvg (rknzcyr; n zna rngvat crnf jvgu gur vqrn gung gurl jvyy "
    "vzcebir uvf ivevyvgl fubiryf gurz fgenvtug vagb uvf ync)",
    "Fuhg gur qbbe naq yvfgra sebz bhgfvqr",
    "Fvzcyl n znggre bs jbex",
    "Fvzcyr Fhogenpgvba",
    "Fvzcyr fhogenpgvba",
    "Fybj cercnengvba, snfg rkrphgvba",
    "Gbjneqf gur vafvtavsvpnag",
    "Gehfg va gur lbh bs abj",
    "Gel snxvat vg (sebz Fgrjneg Oenaq)",
    "Ghea vg hcfvqr qbja",
    "Gjvfg gur fcvar",
    "Gncr lbhe zbhgu (tvira ol Evgin Fnnevxxb)",
    "Gnxr n oernx",
    "Gnxr njnl gur ryrzragf va beqre bs nccnerag aba-vzcbegnapr",
    "Gnxr njnl gur vzcbegnag cnegf",
    "Gur gncr vf abj gur zhfvp",
    "Gur vapbafvfgrapl cevapvcyr",
    "Gur zbfg rnfvyl sbetbggra guvat vf gur zbfg vzcbegnag",
    "Gur zbfg vzcbegnag guvat vf gur guvat zbfg rnfvyl sbetbggra",
    "Guvax - vafvqr gur jbex -bhgfvqr gur jbex",
    "Guvax bs gur enqvb",
    "Gvql hc",
    "Hfr 'hadhnyvsvrq' crbcyr",
    "Hfr fbzrguvat arneol nf n zbqry",
    "Hfr lbhe bja vqrnf",
    "Hfr na byq vqrn",
    "Hfr na hanpprcgnoyr pbybhe",
    "Hfr pyvpurf",
    "Hfr srjre abgrf",
    "Hfr svygref",
    "Ibvpr lbhe fhfcvpvbaf",
    "Jbex ng n qvssrerag fcrrq",
    "Jbhyq nalbar jnag vg?",
    "Jbhyq nalobql jnag vg?",
    "Jngre",
    "Jung gb vapernfr? Jung gb erqhpr? Jung gb znvagnva?",
    "Jung jbhyq lbhe pybfrfg sevraq qb?",
    "Jung jbhyqa'g lbh qb?",
    "Jung jrer lbh ernyyl guvaxvat nobhg whfg abj?",
    "Jung ner gur frpgvbaf frpgvbaf bs? Vzntvar n pngrecvyyne zbivat",
    "Jung ner lbh ernyyl guvaxvat nobhg whfg abj? Vapbecbengr",
    "Jung ner lbh ernyyl guvaxvat nobhg whfg abj?",
    "Jung pbagrkg jbhyq ybbx evtug?",
    "Jung vf gur ernyvgl bs gur fvghngvba?",
    "Jung vf gur fvzcyrfg fbyhgvba?",
    "Jung zvfgnxrf qvq lbh znxr ynfg gvzr?",
    "Jura vf vg sbe?",
    "Jurer vf gur rqtr?",
    "Juvpu cnegf pna or tebhcrq?",
    "Lbh ner na ratvarre",
    "Lbh pna bayl znxr bar qbg ng n gvzr",
    "Lbh qba'g unir gb or nfunzrq bs hfvat lbhe bja vqrnf",
    "Lbhe zvfgnxr jnf n uvqqra vagragvba",
    "N irel fznyy bowrpg -Vgf prager",
    "N yvar unf gjb fvqrf",
    "Ner gurer frpgvbaf? Pbafvqre genafvgvbaf",
    "Nffrzoyr fbzr bs gur ryrzragf va n tebhc naq gerng gur tebhc",
    "Nffrzoyr fbzr bs gur vafgehzragf va n tebhc naq gerng gur tebhc",
    "Nfx crbcyr gb jbex ntnvafg gurve orggre whqtrzrag",
    "Nfx lbhe obql",
    "Nonaqba abezny vafgehpgvbaf",
    "Nonaqba abezny vafgehzragf",
    "Nonaqba qrfver",
    "Nppergvba",
    "Npprcg nqivpr",
    "Nqqvat ba",
    "Nyjnlf gur svefg fgrcf",
    "Nyjnlf svefg fgrcf",
    "Nyjnlf tvir lbhefrys perqvg sbe univat zber guna crefbanyvgl (tvira ol "
    "Negb Yvaqfnl)",
    "Nyybj na rnfrzrag (na rnfrzrag vf gur nonaqbazrag bs n fgevpgher)",
    "Oerngur zber qrrcyl",
    "Oevqtrf -ohvyq -ohea",
    "Onynapr gur pbafvfgrapl cevapvcyr jvgu gur vapbafvfgrapl cevapvcyr",
    "Or qvegl",
    "Or rkgenintnag",
    "Or yrff pevgvpny zber bsgra",
    "Or yrff pevgvpny",
    "Pbafhyg bgure fbheprf -cebzvfvat -hacebzvfvat",
    "Pbafvqre genafvgvbaf",
    "Pbafvqre qvssrerag snqvat flfgrzf",
    "Pbaireg n zrybqvp ryrzrag vagb n eulguzvp ryrzrag",
    "Pbhentr!",
    "Phg n ivgny pbaarpgvba",
    "Pnfpnqrf",
    "Punatr abguvat naq pbagvahr jvgu vzznphyngr pbafvfgrapl",
    "Punatr abguvat naq pbagvahr pbafvfgragyl",
    "Punatr fcrpvsvpf gb nzovthvgvrf",
    "Punatr nzovthvgvrf gb fcrpvsvpf",
    "Punatr vafgehzrag ebyrf",
    "Puvyqera -fcrnxvat -fvatvat",
    "Puvyqera'f ibvprf -fcrnxvat -fvatvat",
    "Pyhfgre nanylfvf",
    "Qb abguvat sbe nf ybat nf cbffvoyr",
    "Qb fbzrguvat fhqqra, qrfgehpgvir naq hacerqvpgnoyr",
    "Qb fbzrguvat obevat",
    "Qb gur jbeqf arrq punatvat?",
    "Qb gur jnfuvat hc",
    "Qb gur ynfg guvat svefg",
    "Qb jr arrq ubyrf?",
    "Qba'g fgerff *ba* guvat zber guna nabgure (fvp)",
    "Qba'g fgerff bar guvat zber guna nabgure",
    "Qba'g nibvq jung vf rnfl",
    "Qba'g oernx gur fvyrapr",
    "Qba'g or nsenvq bs guvatf orpnhfr gurl'er rnfl gb qb",
    "Qba'g or sevtugrarq bs pyvpurf",
    "Qba'g or sevtugrarq gb qvfcynl lbhe gnyragf",
    "Qrfgebl -abguvat -gur zbfg vzcbegnag guvat",
    "Qrfgebl abguvat; Qrfgebl gur zbfg vzcbegnag guvat",
    "Qrpbengr, qrpbengr",
    "Qrsvar na nern nf 'fnsr' naq hfr vg nf na napube",
    "Qvfcynl lbhe gnyrag",
    "Qvfgbeg gvzr",
    "Qvfgbegvat gvzr",
    "Qvfpbaarpg sebz qrfver",
    "Qvfpbire gur erpvcrf lbh ner hfvat naq nonaqba gurz",
    "Qvfpbire lbhe sbezhynf naq nonaqba gurz",
    "Qvfpneq na nkvbz",
    "Qvfpvcyvarq frys-vaqhytrapr",
    "Rzcunfvfr ercrgvgvbaf",
    "Rzcunfvfr gur synjf",
    "Rzcunfvfr qvssreraprf",
    "Rzcunfvmr gur synjf",
    "Rzcunfvmr qvssreraprf",
    "Sebz abguvat gb zber guna abguvat",
    "Snprq jvgu n pubvpr, qb obgu (sebz Qvrgre Ebg)",
    "Snprq jvgu n pubvpr, qb obgu (tvira ol Qvrgre Ebg)",
    "Srrq gur erpbeqvat onpx bhg bs gur zrqvhz",
    "Srrqonpx erpbeqvatf vagb na npbhfgvp fvghngvba",
    "Svaq n fnsr cneg naq hfr vg nf na napube",
    "Svyy rirel orng jvgu fbzrguvat",
    "Tb bhgfvqr. Fuhg gur qbbe.",
    "Tb fybjyl nyy gur jnl ebhaq gur bhgfvqr",
    "Tb gb na rkgerzr, pbzr cneg jnl onpx",
    "Tb gb na rkgerzr, zbir onpx gb n zber pbzsbegnoyr cynpr",
    "Trg lbhe arpx znffntrq",
    "Tubfg rpubrf",
    "Tvir gur tnzr njnl",
    "Tvir jnl gb lbhe jbefg vzchyfr",
    "Ubabe gul reebe nf n uvqqra vagragvba",
    "Ubj jbhyq fbzrbar ryfr qb vg?",
    "Ubj jbhyq lbh unir qbar vg?",
    "Uhznavfr fbzrguvat serr bs reebe",
    "Va gbgny qnexarff, be va n irel ynetr ebbz, irel dhvrgyl",
    "Vagb gur vzcbffvoyr",
    "Vagragvbaf -abovyvgl bs -uhzvyvgl bs -perqvovyvgl bs",
    "Vagragvbaf -perqvovyvgl bs -abovyvgl bs -uhzvyvgl bs",
    "Vasvavgrfvzny tenqngvbaf",
    "Vf fbzrguvat zvffvat?",
    "Vf gur fglyr evtug?",
    "Vf gur ghavat nccebcevngr?",
    "Vf gur ghavat vagbangvba pbeerpg?",
    "Vf gur vagbangvba pbeerpg?",
    "Vf gurer fbzrguvat zvffvat?",
    "Vf vg svavfurq?",
    "Vg vf dhvgr cbffvoyr (nsgre nyy)",
    "Vg vf fvzcyl n znggre be jbex",
    "Vqvbg tyrr (?)",
    "Vzntvar gur cvrpr nf n frg bs qvfpbaarpgrq riragf",
    "Vzntvar gur zhfvp nf n frg bs qvfpbaarpgrq riragf",
    "Vzntvar gur zhfvp nf n zbivat punva be pngrecvyyne",
    "Whfg pneel ba",
    "Ybbx ng gur beqre va juvpu lbh qb guvatf",
    "Ybbx ng n irel fznyy bowrpg, ybbx ng vgf prager",
    "Ybbx pybfryl ng gur zbfg rzoneenffvat qrgnvyf naq nzcyvsl gurz",
    "Ybfg va hfryrff greevgbel",
    "Ybjrfg pbzzba qrabzvangbe purpx -fvatyr orng -fvatyr abgr -fvatyr evss",
    "Ybjrfg pbzzba qrabzvangbe",
    "Yrsg punaary, evtug punaary, prager punaary",
    "Yvfgra gb gur dhvrg ibvpr",
    "Yvfgra va gbgny qnexarff, be va n irel ynetr ebbz, irel dhvrgyl",
    "Zbir gbjneqf gur havzcbegnag",
    "Zhgr naq pbagvahr",
    "Zntavsl gur zbfg qvssvphyg qrgnvyf",
    "Znxr jung'f cresrpg zber uhzna",
    "Znxr n fhqqra, qrfgehpgvir hacerqvpgnoyr npgvba; vapbecbengr",
    "Znxr n oynax inyhnoyr ol chggvat vg va na rkdhvfvgr senzr",
    "Znxr na rkunhfgvir yvfg bs rirelguvat lbh zvtug qb naq qb gur ynfg guvat "
    "ba gur yvfg",
    "Znxr vg zber frafhny",
    "Zrpunavpnyvfr fbzrguvat vqvbflapengvp",
    "[oynax juvgr pneq]",
};

static const size_t N_STRATEGIES = sizeof(s_strategies) / sizeof(const char *);

// =============================================================================
// Public code

void strategies_choose_and_print() {
  static char buf[MAX_STRATEGY_LEN];

  // select a random strategy
  int i = mu_random_range(0, N_STRATEGIES);
  const char *s = s_strategies[i];
  int j = 0;

  // decode (super secret cryptology!)
  while (buf[j++] = rot13(*s++)) {
  }

  // print it
  mu_stddemo_printf("%s\n", buf);
}

// =============================================================================
// Local (static) code

static char rot13(char c) {
  if (((c >= 'a') && (c <= 'm')) || ((c >= 'A') && (c <= 'M'))) {
    return c + 13;
  } else if (((c >= 'n') && (c <= 'z')) || ((c >= 'N') && (c <= 'Z'))) {
    return c - 13;
  } else {
    return c;
  }
}
