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
#include <mulib.h>
#include <stdio.h>
#include <stdlib.h>

// =============================================================================
// Local types and definitions

#define MAX_STRATEGY_LEN 130

// =============================================================================
// Local (forward) declarations

static char rot13(char c);
static void print_string_expressively(char *s);

// =============================================================================
// Local storage

static char const *s_strategies[] = {
  "(Betnavp) znpuvarel.",
  "Abg ohvyqvat n jnyy; znxvat n oevpx.",
  "Bapr gur frnepu unf ortha, fbzrguvat jvyy or sbhaq.",
  "Bayl bar ryrzrag bs rnpu xvaq.",
  "Bayl n cneg, abg gur jubyr.",
  "Biregyl erfvfg punatr.",
  "Chg va rnecyhtf.",
  "Dhrfgvba gur urebvp.",
  "Ercrgvgvba vf n sbez bs punatr.",
  "Ergenpr lbhe fgrcf.",
  "Erinyhngvba (n jnez srryvat).",
  "Erirefr.",
  "Erzbir fcrpvsvpf naq pbaireg gb nzovthvgvrf.",
  "Erzbir n erfgevpgvba.",
  "Erzrzore gubfr dhvrg riravatf.",
  "Fcrpgehz nanylfvf.",
  "Fgngr gur ceboyrz nf pyrneyl nf cbffvoyr.",
  "Fubeg pvephvg.",
  "Fuhg gur qbbe naq yvfgra sebz bhgfvqr.",
  "Fvzcyl n znggre bs jbex.",
  "Fvzcyr Fhogenpgvba.",
  "Fybj cercnengvba, snfg rkrphgvba.",
  "Gbjneqf gur vafvtavsvpnag.",
  "Gehfg va gur lbh bs abj.",
  "Gel snxvat vg.",
  "Ghea vg hcfvqr qbja.",
  "Gjvfg gur fcvar.",
  "Gncr lbhe zbhgu.",
  "Gnxr n oernx.",
  "Gnxr njnl gur ryrzragf va beqre bs nccnerag aba-vzcbegnapr.",
  "Gnxr njnl gur vzcbegnag cnegf.",
  "Gur gncr vf abj gur zhfvp.",
  "Gur vapbafvfgrapl cevapvcyr.",
  "Gur zbfg vzcbegnag guvat vf gur guvat zbfg rnfvyl sbetbggra.",
  "Guvax - vafvqr gur jbex -bhgfvqr gur jbex.",
  "Guvax bs gur enqvb.",
  "Gvql hc.",
  "Hfr 'hadhnyvsvrq' crbcyr.",
  "Hfr fbzrguvat arneol nf n zbqry.",
  "Hfr lbhe bja vqrnf.",
  "Hfr na byq vqrn.",
  "Hfr na hanpprcgnoyr pbybhe.",
  "Hfr pyvpurf.",
  "Hfr srjre abgrf.",
  "Hfr svygref.",
  "Ibvpr lbhe fhfcvpvbaf.",
  "Jbex ng n qvssrerag fcrrq.",
  "Jbhyq nalbar jnag vg?",
  "Jngre.",
  "Jung gb vapernfr? Jung gb erqhpr? Jung gb znvagnva?",
  "Jung jbhyq lbhe pybfrfg sevraq qb?",
  "Jung jbhyqa'g lbh qb?",
  "Jung jrer lbh ernyyl guvaxvat nobhg whfg abj?",
  "Jung pbagrkg jbhyq ybbx evtug?",
  "Jung vf gur ernyvgl bs gur fvghngvba?",
  "Jung vf gur fvzcyrfg fbyhgvba?",
  "Jung zvfgnxrf qvq lbh znxr ynfg gvzr?",
  "Jura vf vg sbe?",
  "Jurer vf gur rqtr?",
  "Juvpu cnegf pna or tebhcrq?",
  "Lbh ner na ratvarre.",
  "Lbh pna bayl znxr bar qbg ng n gvzr.",
  "Lbh qba'g unir gb or nfunzrq bs hfvat lbhe bja vqrnf.",
  "Lbhe zvfgnxr jnf n uvqqra vagragvba.",
  "N irel fznyy bowrpg - vgf prager.",
  "N yvar unf gjb fvqrf.",
  "Ner gurer frpgvbaf? Pbafvqre genafvgvbaf.",
  "Nffrzoyr fbzr bs gur ryrzragf va n tebhc naq gerng gur tebhc.",
  "Nfx crbcyr gb jbex ntnvafg gurve orggre whqtrzrag.",
  "Nfx lbhe obql.",
  "Nonaqba abezny vafgehzragf.",
  "Nonaqba qrfver.",
  "Nppergvba.",
  "Npprcg nqivpr.",
  "Nqqvat ba.",
  "Nyjnlf svefg fgrcf.",
  "Nyybj na rnfrzrag.",
  "Oerngur zber qrrcyl.",
  "Oevqtrf: ohvyq; ohea.",
  "Onynapr gur pbafvfgrapl cevapvcyr jvgu gur vapbafvfgrapl cevapvcyr.",
  "Or qvegl.",
  "Or rkgenintnag.",
  "Or yrff pevgvpny.",
  "Pbafhyg bgure fbheprf -cebzvfvat -hacebzvfvat.",
  "Pbafvqre genafvgvbaf.",
  "Pbafvqre qvssrerag snqvat flfgrzf.",
  "Pbaireg n zrybqvp ryrzrag vagb n eulguzvp ryrzrag.",
  "Pbhentr!",
  "Phg n ivgny pbaarpgvba.",
  "Pnfpnqrf.",
  "Punatr abguvat naq pbagvahr pbafvfgragyl.",
  "Punatr fcrpvsvpf gb nzovthvgvrf.",
  "Punatr nzovthvgvrf gb fcrpvsvpf.",
  "Punatr vafgehzrag ebyrf.",
  "Puvyqera -fcrnxvat -fvatvat.",
  "Pyhfgre nanylfvf.",
  "Qb abguvat sbe nf ybat nf cbffvoyr.",
  "Qb fbzrguvat fhqqra, qrfgehpgvir naq hacerqvpgnoyr.",
  "Qb fbzrguvat obevat.",
  "Qb gur jbeqf arrq punatvat?",
  "Qb gur jnfuvat hc.",
  "Qb gur ynfg guvat svefg.",
  "Qb jr arrq ubyrf?",
  "Qba'g fgerff bar guvat zber guna nabgure.",
  "Qba'g nibvq jung vf rnfl.",
  "Qba'g oernx gur fvyrapr.",
  "Qba'g or nsenvq bs guvatf orpnhfr gurl'er rnfl gb qb.",
  "Qba'g or sevtugrarq bs pyvpurf.",
  "Qba'g or nsenvq gb qvfcynl lbhe gnyragf.",
  "Qrfgebl abguvat; Qrfgebl gur zbfg vzcbegnag guvat.",
  "Qrpbengr, qrpbengr.",
  "Qrsvar na nern nf 'fnsr' naq hfr vg nf na napube.",
  "Qvfcynl lbhe gnyrag.",
  "Qvfgbeg gvzr.",
  "Qvfgbegvat gvzr.",
  "Qvfpbaarpg sebz qrfver.",
  "Qvfpbire gur erpvcrf lbh ner hfvat naq nonaqba gurz.",
  "Qvfpneq na nkvbz.",
  "Qvfpvcyvarq frys-vaqhytrapr.",
  "Rzcunfvfr ercrgvgvbaf.",
  "Rzcunfvfr gur synjf.",
  "Rzcunfvfr qvssreraprf.",
  "Sebz abguvat gb zber guna abguvat.",
  "Snprq jvgu n pubvpr, qb obgu.",
  "Srrqonpx erpbeqvatf vagb na npbhfgvp fvghngvba.",
  "Svaq n fnsr cneg naq hfr vg nf na napube.",
  "Svyy rirel orng jvgu fbzrguvat.",
  "Tb bhgfvqr. Fuhg gur qbbe..",
  "Tb fybjyl nyy gur jnl ebhaq gur bhgfvqr.",
  "Tb gb na rkgerzr, pbzr cneg jnl onpx.",
  "Trg lbhe arpx znffntrq.",
  "Tubfg rpubrf.",
  "Tvir gur tnzr njnl.",
  "Tvir jnl gb lbhe jbefg vzchyfr.",
  "Ubabe gul reebe nf n uvqqra vagragvba.",
  "Ubj jbhyq fbzrbar ryfr qb vg?",
  "Ubj jbhyq lbh unir qbar vg?",
  "Uhznavfr fbzrguvat serr bs reebe.",
  "Va gbgny qnexarff, be va n irel ynetr ebbz, irel dhvrgyl.",
  "Vagb gur vzcbffvoyr.",
  "Vasvavgrfvzny tenqngvbaf.",
  "Vf gur fglyr evtug?",
  "Vf gur ghavat nccebcevngr?",
  "Vf gur ghavat vagbangvba pbeerpg?",
  "Vf gur vagbangvba pbeerpg?",
  "Vf gurer fbzrguvat zvffvat?",
  "Vf vg svavfurq?",
  "Vg vf fvzcyl n znggre be jbex.",
  "Vqvbg tyrr (?).",
  "Vzntvar gur zhfvp nf n frg bs qvfpbaarpgrq riragf.",
  "Whfg pneel ba.",
  "Ybbx ng gur beqre va juvpu lbh qb guvatf.",
  "Ybbx ng n irel fznyy bowrpg, ybbx ng vgf prager.",
  "Ybbx pybfryl ng gur zbfg rzoneenffvat qrgnvyf naq nzcyvsl gurz.",
  "Ybfg va hfryrff greevgbel.",
  "Ybjrfg pbzzba qrabzvangbe.",
  "Yrsg punaary, evtug punaary, prager punaary.",
  "Yvfgra gb gur dhvrg ibvpr.",
  "Yvfgra va gbgny qnexarff, be va n irel ynetr ebbz, irel dhvrgyl.",
  "Zbir gbjneqf gur havzcbegnag.",
  "Zhgr naq pbagvahr.",
  "Zntavsl gur zbfg qvssvphyg qrgnvyf.",
  "Znxr jung'f cresrpg zber uhzna.",
  "Znxr n fhqqra, qrfgehpgvir hacerqvpgnoyr npgvba; vapbecbengr.",
  "Znxr n oynax inyhnoyr ol chggvat vg va na rkdhvfvgr senzr.",
  "Znxr na rkunhfgvir yvfg bs rirelguvat lbh zvtug qb naq qb gur ynfg guvat.",
  "Znxr vg zber frafhny.",
  "Zrpunavpnyvfr fbzrguvat vqvbflapengvp.",
  "[oynax juvgr pneq].",
  "Nonaqba qrfver.",
  "Nonaqba abezny vafgehpgvbaf.",
  "Npprcg nqivpr.",
  "Nqqvat ba.",
  "N yvar unf gjb fvqrf.",
  "Nyjnlf gur svefg fgrcf.",
  "Nfx crbcyr gb jbex ntnvafg gurve orggre whqtrzrag.",
  "Nfx lbhe obql.",
  "Or qvegl.",
  "Or rkgenintnag.",
  "Or yrff pevgvpny.",
  "Oerngur zber qrrcyl.",
  "Oevqtrf -ohvyq -ohea.",
  "Punatr nzovthvgvrf gb fcrpvsvpf.",
  "Punatr abguvat naq pbagvahr pbafvfgragyl.",
  "Punatr fcrpvsvpf gb nzovthvgvrf.",
  "Pbafvqre genafvgvbaf.",
  "Pbhentr!",
  "Phg n ivgny pbaarpgvba.",
  "Qrpbengr, qrpbengr.",
  "Qrfgebl abguvat; Qrfgebl gur zbfg vzcbegnag guvat.",
  "Qvfpneq na nkvbz.",
  "Qvfpvcyvarq frys-vaqhytrapr.",
  "Qvfpbire lbhe sbezhynf naq nonaqba gurz.",
  "Qvfcynl lbhe gnyrag.",
  "Qvfgbeg gvzr.",
  "Qb abguvat sbe nf ybat nf cbffvoyr.",
  "Qba'g nibvq jung vf rnfl.",
  "Qba'g oernx gur fvyrapr.",
  "Qba'g fgerff bar guvat zber guna nabgure.",
  "Qb fbzrguvat obevat.",
  "Qb fbzrguvat fhqqra, qrfgehpgvir naq hacerqvpgnoyr.",
  "Qb gur ynfg guvat svefg.",
  "Qb gur jbeqf arrq punatvat?",
  "Rzcunfvmr qvssreraprf.",
  "Rzcunfvmr gur synjf.",
  "Svaq n fnsr cneg naq hfr vg nf na napube.",
  "Tvir gur tnzr njnl."
};
static const size_t N_STRATEGIES = sizeof(s_strategies) / sizeof(const char *);

// =============================================================================
// Public code

void strategies_choose_and_print() {
  static char buf[MAX_STRATEGY_LEN];
  int index = mu_random_range(0, N_STRATEGIES);
  const char *s = s_strategies[index];

  // decode (super secret cryptology!)
  int j = 0;
  while ((buf[j++] = rot13(*s++))) {
  }

  // print it
  print_string_expressively(buf);
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

static void print_string_expressively(char *s) {
  int screen_width = mu_ansi_term_get_ncols();
  // mu_ansi_term_clear_screen();
  int indent_by = mu_random_range(0, screen_width - strlen(s));

  while(indent_by--)
    printf(" ");

  mu_ansi_term_set_colors(mu_random_range(0,MU_ANSI_COLOR_COUNT),MU_ANSI_TERM_DEFAULT_COLOR);

  printf("%s\n", s);
      
  mu_ansi_term_set_colors(MU_ANSI_TERM_DEFAULT_COLOR, MU_ANSI_TERM_DEFAULT_COLOR);

}


