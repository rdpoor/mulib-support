/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
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

#include "hexagrams.h"

    
#include <mulib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// =============================================================================
// Local types and definitions


// =============================================================================
// Local storage

static const char *_line_strings[] = {
  "------  -  -------", // 6 = old yin
  "------------------", // 7 = young yang
  "------     -------", // 8 = young yin
  "------+++++-------", // 9 = old yang 
};



static char changed_user_lines[7];

// Wilhelm translation
static const i_ching_hexagram  hexagrams[] = {
  {
    .number = 1,
    .sk = 0b00111111, // rightmost bit is the foundation (bottom line in the hexagram)
    .name = "The Creative",
    .cm = "The first hexagram is made up of six unbroken lines. These unbroken\nlines stand for the primal power, which is light-giving, active, strong,\nand of the spirit. The hexagram is consistently strong in character, and\nsince it is without weakness, its essence is power or energy. Its image\nis heaven. Its energy is represented as unrestricted by any fixed conditions\nin space and is therefore conceived of as motion. Time is regarded as the\nbasis of this motion. Thus the hexagram includes also the power of time\nand the power of persisting in time, that is, duration. The power represented\nby the hexagram is to be interpreted in a dual sense in terms of its action\non the universe and of its action on the world of men. In relation to the\nuniverse, the hexagram expresses the strong, creative action of the Deity.\nIn relation to the human world, it denotes the creative action of the holy\nman or sage, of the ruler or leader of men, who through his power awakens\nand develops their higher nature.",
    .jd = "THE CREATIVE works sublime success, Furthering through perseverance.",
    .j_cm = "According to the original meaning, the attributes [sublimity, potentiality\nof success, power to further, perseverance] are paired. When an individual\ndraws this oracle, it means that success will come to him from the primal\ndepths of the universe and that everything depends upon his seeking his\nhappiness and that of others in one way only, that is, by perseverance\nin what is right. The specific meanings of the four attributes became\nthe subject of speculation at an early date. The Chinese word here rendered\nby \"sublime\" means literally \"head,\" \"origin,\" \"great.\" This is why Confucius\nsays in explaining it: \"Great indeed is the generating power of the Creative;\nall beings owe their beginning to it. This power permeates all heaven.\"\nFor this attribute inheres in the other three as well. The beginning of\nall things lies still in the beyond in the form of ideas that have yet to\nbecome real. But the Creative furthermore has power to lend form to these\narchetypes of ideas. This is indicated in the word success, and the process\nis represented by an image from nature: \"The clouds pass and the rain does\nits work, and all individual beings flow into their forms.\" Applies to\nthe human world, these attributes show the great man the way to notable\nsuccess: \"Because he sees with great clarity and cause and effects, he completes\nthe six steps at the right time and mounts toward heaven on them at the\nright time, as though on six dragons.\" The six steps are the six different\npositions given in the hexagram, which are represented later by the dragon\nsymbol. Here it is shown that the way to success lies in apprehending and\ngiving actuality to the way of the universe [Tao], which, as a law running\nthrough end and beginning, brings about all phenomena in time. Thus each\nstep attained forthwith becomes a preparation for the next. Time is no longer\na hindrance but the means of making actual what is potential. The act of\ncreation having found expression in the two attributes sublimity and success,\nthe work of conservation is shown to be a continuous\nactualization and differentiation of form.\n\nThis is expressed in the two terms \"furthering\" (literally, \"creating\nthat which accords with the nature of a given being\") and \"persevering\"\n(literally, \"correct and firm\"). \"The course of the Creative alters and\nshapes beings until each attains its true, specific nature, then it keeps\nthem in conformity with the Great Harmony. Thus does it show itself to further\nthrough perseverance.\" In relation to the human sphere, this shows how the\ngreat man brings peace and security to the world through his activity in\ncreating order: \"He towers high above the multitude of beings, and all lands\nare united in peace.\" Another line of speculation goes still further in\nseparating the words \"sublime,\" \"success,\" \"furthering,\" \"perseverance,\"\nand parallels them with the four cardinal virtues in humanity. To sublimity,\nwhich, as the fundamental principle, embraces all the other attributes,\nit links love. To the attribute success are linked the morals, which regulate\nand organize expressions of love and thereby make them successful. The attribute\nfurthering is correlated with justice, which creates the conditions in which\neach receives that which accords with his being, that which is due him and\nwhich constitutes his happiness. The attribute perseverance is correlated\nwith wisdom, which discerns the immutable laws of all that happens and can\ntherefore bring about enduring conditions. These speculations, already broached\nin the commentary called Wên Yen , later formed the bridge connecting\nthe philosophy of the \"five stages (elements) of change,\" as laid down in\nthe Book of History (Shu Ching) with the philosophy of the Book of Changes,\nwhich is based solely on the polarity of positive and negative principles.\nIn the course of time this combination of the two systems of thought opened\nthe way for an increasingly intricate number symbolism.",
    .im = "The movement of heaven is full of power.\nThus the superior man makes himself strong and untiring.",
    .i_cm = "Since there is only one heaven, the doubling of the trigram Ch'ien,\nof which heaven is the image, indicates the movement of heaven. One complete\nrevolution of heaven makes a day, and the repetition of the trigram means\nthat each day is followed by another. This creates the idea of time.\nSince it is the same heaven moving with untiring power, there is also\ncreated the idea of duration both in and beyond time, a movement that\nnever stops nor slackens, just as one day follows another in an unending\ncourse. This duration in time is the image of the power inherent in the\nCreative. With this image as a model, the sage learns how best to develop\nhimself so that his influence may endure. He must make himself strong in\nevery way, by consciously casting out all that is inferior and degrading.\nThus he attains that tirelessness which depends upon consciously limiting\nthe fields of his activity.",
    .lines = {
        {
          .pd ="Hidden dragon. Do not act.",
          .cm = "In China the dragon has a meaning altogether different from that given\nit in the Western world. The dragon is a symbol of the electrically charged,\ndynamic, arousing force that manifests itself in the thunderstorm. In\nwinter this energy withdraws into the earth; in the early summer it becomes\nactive again, appearing in the sky as thunder and lightning. As a result\nthe creative forces on earth begin to stir again. Here this creative\nforce is still hidden beneath the earth and therefore has no effect. In\nterms of human affairs, this symbolizes a great man who is still unrecognized.\nNonetheless he remains true to himself. He does not allow himself to be\ninfluenced by outward success or failure, but confident in his strength,\nhe bides his time. Hence it is wise for the man who consults the oracle\nand draws this line to wait in the calm strength of patience. The time will\nfulfill itself. One need not fear least strong will should not prevail;\nthe main thing is not to expend one's powers prematurely in an attempt to\nobtain by force something for which the time is not yet ripe.",
        },
        {
          .pd ="Dragon appearing in the field.\nIt furthers one to see the great man.",
          .cm = "Here the effects of the light-giving power begin to manifest themselves.\nIn terms of human affairs, this means that the great man makes his appearance\nin his chosen field of activity. As yet he has no commanding position\nbut is still with his peers. However, what distinguishes him form the\nothers is his seriousness of purpose, his unqualified reliability, and\nthe influence he exerts on his environment with out conscious effort.\nSuch a man is destined to gain great influence and to set the world in\norder. Therefore it is favorable to see him.",
        },
        {
          .pd ="All day long the superior man is creatively active.\nAt nightfall his mind is still beset with cares.\nDanger. No blame.",
          .cm = "A sphere of influence opens up for the great man. His fame begins\nto spread. The masses flock to him. His inner power is adequate to the increased\nouter activity. There are all sorts of things to be done, and when others\nare at rest in the evening, plans and anxieties press in upon him. But\ndanger lurks here at the place of transition from lowliness to the heights.\nMany a great man has been ruined because the masses flocked to him and\nswept him into their course. Ambition has destroyed his integrity. However,\ntrue greatness is not impaired by temptations. He who remains in touch with\nthe time that is dawning, and with its demands is prudent enough to avoid\nall pitfalls, and remains blameless.",
        },
        {
          .pd ="Wavering flight over the depths.\nNo blame.",
          .cm = "A place of transition has been reached, and free choice can enter\nin. A twofold possibility is presented to the great man: he can soar to the\nheights and play an important part in the world, or he can withdraw into\nsolitude and develop himself. He can go the way of the hero or that of the\nholy sage who seeks seclusion. There is no general law of his being. If the\nindividual acts consistently and is true to himself, he will find the way\nthat is appropriate for him. This way is right for him and without blame.",
        },
        {
          .pd ="Flying dragon in the heavens.\nIt furthers one to see the great man.",
          .cm = "Here the great man has attained the sphere of the heavenly beings.\nHis influence spreads and becomes visible throughout the whole world.\nEveryone who sees him may count himself blessed. Confucius says about\nthis line:\n\nThings that accord in tone vibrate together. Things that have affinity\nin their inmost natures seek one another. Water flows to what is wet,\nfire turns to what is dry. Clouds (the breath of heaven) follow the dragon,\nwind (the breath of earth) follows the tiger. Thus the sage arises, and\nall creatures follow him with their eyes. What is born of heaven feels\nrelated to what is above. What is born of earth feels related to what is\nbelow. Each follows its kind.",
        },
        {
          .pd ="Arrogant dragon will have cause to repent.",
          .cm = "When a man seeks to climb so high that he loses touch with the rest\nof mankind, he becomes isolated, and this necessarily leads to failure.\nThis line warns against titanic aspirations that exceed one's power. A\nprecipitous fall would follow.",
        },
        {
          .pd ="There appears a flight of dragons without heads.\nGood fortune.",
          .cm = "When all the lines are nines, it means that the whole hexagram is\nin motion and changes into the hexagram K'un, THE RECEPTIVE, whose character\nis devotion. The strength of the Creative and the mildness of the Receptive\nunite. Strength is indicated by the flight of dragons, mildness by the\nfact that their heads are hidden. This means that mildness in action joined\nto strength of decision brings good fortune.",
        }
      }
  },
  {
    .number = 2,
    .sk = 0b00000000, // rightmost bit is the foundation (bottom line in the hexagram)
    .name = "The Receptive",
    .cm = "This hexagram is made up of broken lines only. The broken lines represents\nthe dark, yielding, receptive primal power of yin. The attribute of the\nhexagram is devotion; its image is the earth. It is the perfect complement\nof THE CREATIVE--the complement, not the opposite, for the Receptive does\nnot combat the Creative but completes it . It represents nature in contrast\nto spirit, earth in contrast to heaven, space as against time,\nthe female-maternal as against the male-paternal.\n\nHowever, as applied to human affairs, the principle of this complementary\nrelationship is found not only in the relation between\nman and woman, but also in that between prince and minister and\nbetween father and son. Indeed, even in the individual this duality appears\nin the coexistence of the spiritual world and the world of the senses.\nBut strictly speaking there is no real dualism here, because there is a\nclearly defined hierarchic relationship between the two principles. In itself\nof course the Receptive is just as important as the Creative, but the attribute\nof devotion defines the place occupied by this primal power in relation\nto the Creative. For the Receptive must be activated and led by the Creative;\nthen it is productive of good. Only when it abandons this position and\ntries to stand as an equal side by side with the Creative, does it become\nevil. The result then is opposition to and struggle against the Creative,\nwhich is productive of evil to both.",
    .jd = "THE RECEPTIVE brings about sublime success,\nFurthering through the perseverance of a mare.\nIf the superior man undertakes something and tries to lead,\nHe goes astray;\nBut if he follows, he finds guidance.\nIt is favorable to find friends in the west and south,\nTo forego friends in the east and north.\nQuiet perseverance brings good fortune.",
    .j_cm = "The four fundamental aspects of the Creative--\"sublime success, furthering\nthrough perseverance\"--are also attributed to the Receptive. Here, however,\nthe perseverance is more closely defined: it is that of a mare. The Receptive\nconnotes spatial reality in contrast to the spiritual potentiality of\nthe Creative. The potential becomes real and the spiritual becomes spatial\nthrough a specifically qualifying definition. Thus the qualification,\n\"of a mare,\" is here added to the idea of perseverance. The horse belongs\nto earth just as the dragon belongs to heaven. Its tireless roaming over\nthe plains is taken as a symbol of the vast expanse of the earth. This\nis the symbol chosen because the mare combines the strength and swiftness\nof the horse with the gentleness and devotion of the cow. Only because\nnature in its myriad forms corresponds with the myriad impulses of the Creative\ncan it make these impulses real. Nature's richness lies in its power to\nnourish all living things; its greatness lies in its power to give then\nbeauty and splendor. Thus it prospers all that lives. IT is the Creative\nthat begets things, but they are brought to birth by the Receptive. Applied\nto human affairs, therefore, what the hexagram indicated is action in conformity\nwith the situation. The person in questions not in an independent position,\nbut is acting as an assistant. This means that he must achieve something.\nIt is not his task to try to lead--that would only make him lose the way-but\nto let himself be led. If he knows how to meet fate with an attitude of\nacceptance, he is sure to find the right guidance. The superior man lets\nhimself be guided; he does not go ahead blindly, but learns from the situation\nwhat is demanded of him and then follows this intimation from fate. Since\nthere is something to be accomplished, we need friends and helpers in the\nhour of toil and effort, once the ideas to be realized are firmly set. The\ntime of toil and effort is indicated by the west and south, for west and\nsouth symbolize the place where the Receptive works for the Creative, as\nnature does in summer and autumn. If in that situation one does not mobilize\nall one's powers, the work to be accomplished will not be done. Hence to\nfind friends there means to find guidance. But in addition to the time\nof toil and effort, there is also a time of planning, and for this we need\nthis solitude. The east symbolized the place where a man receives orders\nfrom his master, and the north the place where he reports on what he has\ndone. At that time he must be alone and objective. In this sacred hour he\nmust do without companions. So that the purity of the moment may not be spoiled\nby fictional hates and favoritism."
        "The earth's condition is receptive devotion.\nThus the superior man who has breadth of character\nCarries the outer world.",
    .im = "The earth's condition is receptive devotion.\nThus the superior man who has breadth of character\nCarries the outer world.",
    .i_cm = "Just as there is only one heaven, so too there is only one earth.\nIn the hexagram of heaven the doubling of the trigram implies duration in\ntime, but in the hexagram of earth the doubling connotes the solidity and\nextension in space by virtue of which the earth is able to carry and preserve\nall things that live and move upon it. The earth in its devotion carries\nall things, good and evil,, without exception. In the same way the superior\nman gives to his character breadth, purity, and sustaining power, so that\nhe is able both to support and to bear with people and things.",
    .lines = {
        {
          .pd ="When there is hoarfrost underfoot,\nSolid ice is not far off.",
          .cm = "Just as the light-giving power represents life, so the dark power,\nthe shadowy, represents death. When the first hoarfrost comes in the\nautumn, the power of darkness and cold is just at its beginning. After\nthese first warnings, signs of death will gradually multiply, until,\nin obedience to immutable laws, stark winter with its ice is here. In\nlife it is the same. After certain scarcely noticeable signs of decay have\nappeared, they go on increasing until final dissolution comes. But in life\nprecautions can be taken by heeding the first signs of decay and checking\nthem in time.",
        },
        {
          .pd ="Straight, square, great.\nWithout purpose,\nYet nothing remains unfurthered.",
          .cm = "The symbol of heaven is the circle, and that of earth is the square.\nThus squareness is a primary quality of the earth. On the other hand,\nmovement in a straight line, as well as magnitude, is a primary quality\nof the Creative. But all square things have their origin in a straight\nline and into turn form solid bodies. In mathematics, when we discriminate\nbetween lines, planes and solids, we find that rectangular planes result\nfrom straight lines, and cubic magnitudes from rectangular planes. The Receptive\naccommodates itself to the qualities of the Creative and makes them its\nown. Thus a square develops out of a straight line and a cube out of a\nsquare. This is compliance with the laws of the Creative; nothing is taken\naway, nothing added. Therefore the Receptive has no need of a special\npurpose of its own, nor of any effort' yet everything turns out as it\nshould. Nature creates all beings without erring: this is its foursquareness.\nIt tolerates all creatures equally: this is its greatness. Therefore it\nattains what is right for all without artifice or special intentions. Man\nachieves the height of wisdom when all that he does is as self-evident\nas what nature does.",
        },
        {
          .pd ="Hidden lines.\nOne is able to remain persevering.\nIf by chance you are in the service of a king,\nSeek not works, but bring to completion.",
          .cm = "If a man is free of vanity he is able to conceal his abilities and\nkeep them from attracting attention too soon; thus he can mature undisturbed.\nIf conditions demand it, he can also enter public life, but that too\nhe does with restraint. The wise man gladly leaves fame to others. He does\nnot seek to have credited to himself things that stand accomplished, but\nhopes to release active forces; that is, he completes his works in such\na manner that they may bear fruit for the future.",
        },
        {
          .pd ="A tied-up sack. No blame, no praise.",
          .cm = "The dark element opens when it moves and closes when at rest. The\nstrictest reticence is indicated here. The time is dangerous , because any\ndegree of prominence leads either to the enmity of irresistible antagonists\nif one challenges them or to misconceived recognition if one is complaisant.\nTherefore a man ought to maintain reserve, be it in solitude or in the turmoil\nof the world, for there too he can hide himself so well that no one knows\nhim.",
        },
        {
          .pd ="A yellow lower garment brings supreme good fortune.",
          .cm = "Yellow is the color of the earth and of the middle; it is the symbol\nof that which is reliable and genuine. The lower garment is inconspicuously\ndecorated--the symbol of aristocratic reserve. When anyone is called\nupon to work in a prominent but not independent position, true success\ndepends on the utmost discretion. A man's genuineness and refinement\nshould not reveal themselves directly; they should express themselves\nonly indirectly as an effect from within.",
        },
        {
          .pd ="Dragons fight in the meadow.\nTheir blood is black and yellow.",
          .cm = "In the top place the dark element should yield to the light. If it\nattempts to maintain a position to which it is not entitled and to rule\ninstead of serving, it draws down upon itself the anger of the strong.\nA struggle ensues in which it is overthrown, with injury, however, to both\nsides. The dragon, symbol of heaven, comes to fight the false dragon that\nsymbolized the inflation of the earth principle. Midnight blue is the\ncolor of heaven; yellow is the color of earth. Therefore, when black and\nyellow blood flow, it is a sign that in this unnatural contest both primal\npowers suffer injury.",
        },
        {
          .pd ="Lasting perseverance furthers.",
          .cm = "When nothing but sixes appears, the hexagram of THE RECEPTIVE changes\ninto the hexagram of THE CREATIVE. By holding fast to what is right,\nit gains the power of enduring. There is indeed no advance, but neither\nis there retrogression.",
        }
      }
  },
  {
    .number = 3,
    .sk = 0b00010001, // rightmost bit is the foundation (bottom line in the hexagram)
    .name = "Difficulty at the Beginning",
    .cm = "The name of the hexagram, Chun, really connotes a blade of grass pushing\nagainst an obstacle as it sprouts out of the earth--hence the meaning,\n\"difficulty at the beginning.\" The hexagram indicates the way in which\nheaven and earth bring forth individual beings. It is their first meeting,\nwhich is beset with difficulties. The lower trigram Chên is the Arousing;\nits motion is upward and its image is thunder. The upper trigram K'an stands\nfor the Abysmal, the dangerous. Its motion is downward and its image is\nrain. The situation points to teeming, chaotic profusion; thunder and rain\nfill the air. But the chaos clears up. While the Abysmal sinks, the upward\nmovement eventually passes beyond the danger. A thunderstorm brings release\nfrom tension, and all things breathe freely again.",
    .jd = "DIFFICULTY AT THE BEGINNING works supreme success,\nFurthering through perseverance.\nNothing should be undertaken.\nIt furthers one to appoint helpers.",
    .j_cm = "Times of growth are beset with difficulties. They resemble a first\nbirth. But these difficulties arise from the very profusion of all that\nis struggling to attain form . Everything is in motion: therefore if one\nperseveres there is a prospect of great success, in spite of the existing\ndanger. When it is a man's fate to undertake such new beginnings, everything\nis still unformed, dark. Hence he must hold back, because any premature\nmove might bring disaster. Likewise, it is very important not to remain\nalone; in order to overcome the chaos he needs helpers. This is not to say,\nhowever, that he himself should look on passively at what is happening.\nHe must lend his hand and participate with inspiration and guidance.",
    .im =   "Clouds and thunder:\nThe image of DIFFICULTY AT THE BEGINNING.\nThus the superior man\nBrings order out of confusion.",
    .i_cm = "Clouds and thunder are represented by definite decorative lines; this\nmeans that in the chaos of difficulty at the beginning, order is already\nimplicit. So too the superior man has to arrange and organize the inchoate\nprofusion of such times of beginning, just as one sorts out silk threads\nfrom a knotted tangle and binds them into skeins. In order to find one's\nplace in the infinity of being, one must be able both to separate and to\nunite.",
        .lines = {
        {
          .pd ="Hesitation and hindrance.\nIt furthers one to remain persevering.\nIt furthers one to appoint helpers.",
          .cm = "If a person encounters a hindrance at the beginning of an enterprise,\nhe must not try to force advance but must pause and take thought. However,\nnothing should put him off his course; he must persevere and constantly\nkeep the goal in sight. It is important to seek out the right assistants,\nbut he can find them only if he avoids arrogance and associated with his\nfellows in a spirit of humility. Only then will he attract those with whose\nhelp he can combat the difficulties.",
        },
        {
          .pd ="Difficulties pile up.\nHorse and wagon part.\nHe is not a robber;\nHe wants to woo when the time comes.\nThe maiden is chaste,\nShe does not pledge herself.\nTen years--then she pledges herself.",
          .cm = "We find ourselves beset by difficulties and hindrances. Suddenly there\nis a turn of affairs, as if someone were coming up with a horse and wagon\nand unhitching them. This event comes so unexpectedly that we assume\nthe newcomer to be a robber. Gradually it becomes clear that he has no\nevil intentions but seeks to be friendly and to offer help. But this offer\nis not to be accepted, because it does not come from the right quarter.\nWe must wait until the time is fulfilled; ten years is a fulfilled cycle\nof time. Then normal conditions return of themselves, and we can join forces\nwith the friend intended for us. Using the image of a betrothed girl who\nremains true to her lover in face of grave conflicts, the hexagram gives\ncounsel for a special situation. When in times of difficulty a hindrance\nis encountered and unexpected relief is offered from a source unrelated\nto us, we must be careful and not take upon ourselves any obligations entailed\nby such help; otherwise our freedom of decision is impaired. If we bide\nour time, things will quiet down again, and we shall attain what we have\nhoped for.",
        },
        {
          .pd ="Whoever hunts deer without the forester\nOnly loses his way in the forest.\nThe superior man understands the signs of the time\nAnd prefers to desist.\nTo go on brings humiliation.",
          .cm = "If a man tries to hunt in a strange forest and has no guide, he loses\nhis way. When he finds himself in difficulties he must not try to steal\nout of them unthinkingly and without guidance. Fate cannot be duped; premature\neffort, without the necessary guidance, ends in failure and disgrace.\nTherefore the superior man, discerning the seeds of coming events, prefers\nto renounce a wish rather than to provoke failure and humiliation by trying\nto force its fulfillment.",
        },
        {
          .pd ="Horse and wagon part.\nStrive for union.\nTo go brings good fortune.\nEverything acts to further.",
          .cm = "We are in a situation in which it is our duty to act, but we lack\nsufficient power. However, an opportunity to make connections offers itself.\nIt must be seized. Neither false pride nor false reserve should deter us.\nBringing oneself to take the first step, even when it involves a certain\ndegree of self-abnegation, is a sign of inner clarity. To accept help in\na difficult situation is not a disgrace. If the right helper is found, all\ngoes well.",
        },
        {
          .pd ="Difficulties in blessing.\nA little perseverance brings good fortune.\nGreat perseverance brings misfortune.",
          .cm = "An individual is in a position in which he cannot so express his good\nintentions that they will actually take shape and be understood. Other\npeople interpose and distort everything he does. He should then be cautious\nand proceed step by step. He must not try to force the consummation of\na great undertaking, because success is possible only when general confidence\nalready prevails. It is only through faithful and conscientious work, unobtrusively\ncarried on, that the situation gradually clears up and the hindrance disappears.",
        },
        {
          .pd ="Horse and wagon part.\nBloody tears flow.",
          .cm = "The difficulties at the beginning are too great for some persons.\nThey get stuck and never find their way out; they fold their hands and give\nup the struggle. Such resignation is the saddest of all things. Therefore\nConfucius says of this line: \"Bloody tears flow: one should not persist in\nthis.\"",
        }
      }
  },
  {
    .number = 4,
    .sk = 0b00100010, // rightmost bit is the foundation (bottom line in the hexagram)
    .name = "Youthful Folly",
    .cm = "In this hexagram we are reminded of youth and folly in two different\nways. The image of the upper trigram, Kên, is the mountain, that\nof the lower, K'an, is water; the spring rising at the foot of the mountain\nis the image of inexperienced youth. Keeping still is the attribute of\nthe upper trigram; that of the lower is the abyss, danger. Stopping in\nperplexity on the brink of a dangerous abyss is a symbol of the folly of\nyouth. However, the two trigrams also show the way of overcoming the follies\nof youth. Water is something that of necessity flows on. When the spring\ngushes forth, it does not know at first where it will go. But its steady\nflow fills up the deep place blocking its progress, and success is attained.",
    .jd = "YOUTHFUL FOLLY has success.\nIt is not I who seek the young fool;\nThe young fool seeks me.\nAt the first oracle I inform him.\nIf he asks two or three times, it is importunity.\nIf he importunes, I give him no information.\nPerseverance furthers.",
    .j_cm = "In the time of youth, folly is not an evil. One may succeed in spite\nof it, provided one finds an experienced teacher and has the right attitude\ntoward him. This means, first of all, that the youth himself must be\nconscious of his lack of experience and must seek out the teacher. Without\nthis modesty and this interest there is no guarantee that he has the necessary\nreceptivity, which should express itself in respectful acceptance of the\nteacher. This is the reason why the teacher must wait to be sought out\ninstead of offering himself. Only thus can the instruction take place\nat the right time and in the right way. A teacher's answer to the question\nof a pupil ought to be clear and definite like that expected from an oracle;\nthereupon it ought to be accepted as a key for resolution of doubts and\na basis for decision. If mistrustful or unintelligent questioning is kept\nup, it serves only to annoy the teacher. He does well to ignore it in silence,\njust as the oracle gives one answer only and refuses to be tempted by questions\nimplying doubt. Given addition a perseverance that never slackens until\nthe points are mastered one by one, real success is sure to follow. Thus\nthe hexagram counsels the teacher as well as the pupil.",
    .im =   "A spring wells up at the foot of the mountain:\nThe image of YOUTH.\nThus the superior man fosters his character\nBy thoroughness in all that he does.",
    .i_cm = "A spring succeeds in flowing on and escapes stagnation by filling\nup all the hollow places in its path. In the same way character is developed\nby thoroughness that skips nothing but, like water, gradually and steadily\nfills up all gaps and so flows onward.",
        .lines = {
        {
          .pd ="To make a fool develop\nIt furthers one to apply discipline.\nThe fetters should be removed.\nTo go on in this way bring humiliation.",
          .cm = "Law is the beginning of education. Youth in its inexperience is inclined\nat first to take everything carelessly and playfully. It must be shown\nthe seriousness of life. A certain measure of taking oneself in hand,\nbrought about by strict discipline, is a good thing. He who plays with\nlife never amounts to anything. However, discipline should not degenerate\ninto drill. Continuous drill has a humiliating effect and cripples a\nman's powers.",
        },
        {
          .pd ="To bear with fools in kindliness brings good fortune.\nTo know how to take women\nBrings good fortune.\nThe son is capable of taking charge of the household.",
          .cm = "These lines picture a man who has no external power, but who has enough\nstrength of mind to bear his burden of responsibility. He has the inner\nsuperiority and that enable him to tolerate with kindliness the shortcomings\nof human folly. The same attitude is owed to women as the weaker sex.\nOne must understand them and give them recognition in a spirit of chivalrous\nconsideration. Only this combination of inner strength with outer reserve\nenables one to take on the responsibility of directing a larger social\nbody with real success.",
        },
        {
          .pd ="Take not a maiden who, \nWhen she sees a man of bronze,\nLoses possession of herself.\nNothing furthers.",
          .cm = "A weak, inexperienced man, struggling to rise, easily loses his own\nindividuality when he slavishly imitates a strong personality of higher\nstation. He is like a girl throwing herself away when she meets a strong\nman. Such a servile approach should not be encouraged, because it is bad\nboth for the youth and the teacher. A girl owes it to her dignity to wait\nuntil she is wooed. In both cases it is undignified to offer oneself,\nand no good comes of accepting such an offer.",
        },
        {
          .pd ="Entangled folly bring humiliation.",
          .cm = "For youthful folly it is the most hopeless thing to entangle itself\nin empty imaginings. The more obstinately it clings to such unreal fantasies,\nthe more certainly will humiliation overtake it. Often the teacher, when\nconfronted with such entangled folly, has no other course but to leave\nthe fool to himself for a time, not sparing him the humiliation that results.\nThis is frequently the only means of rescue.",
        },
        {
          .pd ="Childlike folly brings good fortune.",
          .cm = "An inexperienced person who seeks instruction in a childlike and unassuming\nway is on the right path, for the man devoid of arrogance who subordinated\nhimself to his teacher will certainly be helped.",
        },
        {
          .pd ="In punishing folly\nIt does not further one\nTo commit transgressions.\nThe only thing that furthers\nIs to prevent transgressions.",
          .cm = "Sometimes an incorrigible fool must be punished. He who will not heed\nwill be made to feel. This punishment is quite different from a preliminary\nshaking up. But the penalty should not be imposed in anger; it must be\nrestricted to an objective guarding against unjustified excesses. Punishment\nis never an end in itself but serves merely to restore order. This applies\nnot only in regard to education but also in regard to the measures taken\nby a government against a populace guilty of transgressions. Governmental\ninterference should always be merely preventive and should have as its\nsole aim the establishment of public security and peace.",
        }
      }
  },
  {
    .number = 5,
    .sk = 0b00010111,
    .name = "Waiting (Nourishment)",
    .cm = "All beings have need of nourishment from above. But the gift of food\ncomes in its own time, and for this one must wait. This hexagram shows\nthe clouds in the heavens, giving rain to refresh all that grows and to\nprovide mankind with food and drink. The rain will come in its own time.\nWe cannot make it come; we have to wait for it. The idea of waiting is\nfurther suggested by the attributes of the two trigrams--strength within,\ndanger in from. Strength in the face of danger does not plunge ahead but\nbides its time, whereas weakness in the face of danger grows agitated and\nhas not the patience to wait.",
    .jd = "WAITING. If you are sincere,\nYou have light and success.\nPerseverance brings good fortune.\nIt furthers one to cross the great water.",
    .j_cm = "Waiting is not mere empty hoping. It has the inner certainty of reaching\nthe goal. Such certainty alone gives that light which leads to success.\nThis leads to the perseverance that brings good fortune and bestows power\nto cross the great water. One is faced with a danger that has to be overcome.\nWeakness and impatience can do nothing. Only a strong man can stand up\nto his fate, for his inner security enables him to endure to the end.\nThis strength shows itself in uncompromising truthfulness [with himself].\nIt is only when we have the courage to face things exactly as they are,\nwithout any sort of self-deception or illusion, that a light will develop\nout of events, by which the path to success may be recognized. This recognition\nmust be followed by resolute and persevering action. For only the man who\ngoes to meet his fate resolutely is equipped to deal with it adequately.\nThen he will be able to cross the great water--that is to say, he will be\ncapable of making the necessary decision and of surmounting the danger.",
    .im =   "Clouds rise up to heaven:\nThe image of WAITING.\nThus the superior man eats and drinks,\nIs joyous and of good cheer.",
    .i_cm = "When clouds rise in the sky, it is a sign that it will rain. There\nis nothing to do but to wait until after the rain falls. It is the same\nin life when destiny is at work. We should not worry and seek to shape\nthe future by interfering in things before the time is ripe. We should\nquietly fortify the body with food and drink and the mind with gladness\nand good cheer. Fate comes when it will, and thus we are ready.",
        .lines = {
        {
          .pd = "Waiting in the meadow.\nIT furthers one to abide in what endures.\nNo blame.",
          .cm = "The danger is not yet close. One is still waiting on the open plain.\nConditions are still simple, yet there is a feeling of something impending.\nOne must continue to lead a regular life as long as possible. Only in\nthis way does one guard against a premature waste of strength, keep free\nof blame and error that would become a source of weakness later on.",
        },
        {
          .pd ="Waiting on the sand.\nThere is some gossip.\nThe end brings good fortune.",
          .cm = "The danger gradually comes closer. Sand is near the bank of the river,\nand the water means danger. Disagreements crop up. General unrest can\neasily develop in such times, and we lay the blame on one another. He\nwho stays calm will succeed in making things go well in the end. Slander\nwill be silenced if we do not gratify it with injured retorts.",
        },
        {
          .pd ="Waiting in the mud\nBrings about the arrival of the enemy.",
          .cm = "Mud is no place for waiting, since it is already being washed by the\nwater of the stream. Instead of having gathered strength to cross the\nstream at one try, one has made a premature start that has got him no\nfarther than the muddy bank. Such an unfavorable position invites enemies\nfrom without, who naturally take advantage of it. Caution and a sense of\nthe seriousness of the situation are all that can keep one from injury.",
        },
        {
          .pd ="Waiting in blood.\nGet out of the pit.",
          .cm = "The situation is extremely dangerous. IT is of utmost gravity now--a\nmatter of life and death. Bloodshed seems imminent. There is no going\nforward or backward; we are cut off as if in a pit. Now we must simply\nstand fast and let fate take its course. This composure, which keeps us\nfrom aggravating the trouble by anything we might do, is the only way\nof getting out of the dangerous pit.",
        },
        {
          .pd ="Waiting at meat and drink.\nPerseverance brings good fortune.",
          .cm = "Even in the midst of danger there come intervals of peace when things\ngo relatively well. If we possess enough inner strength, we shall take\nadvantage of these intervals to fortify ourselves for renewed struggle.\nWe must know how to enjoy the moment without being deflected from the\ngoal, for perseverance is needed to remain victorious. This is true in\npublic life as well; it is not possible to achieve everything all at once.\nThe height of wisdom is to allow people enough recreation to quicken pleasure\nin their work until the task is completed. Herein lies the secret of the\nwhole hexagram. It differs from Chin OBSTRUCTION 39, in the fact that in this instance, while waiting,\nwe are sure of our cause and therefore do not lose the serenity born\nof inner cheerfulness.",
        },
        {
          .pd ="One falls into the pit.\nThree uninvited guests arrive.\nHonor them, and in the end there will be good fortune.",
          .cm = "The waiting is over; the danger can no longer be averted. One falls\ninto the pit and must yield to the inevitable. Everything seems to have\nbeen in vain. But precisely in this extremity things take an unforeseen\nturn. Without a move on one's own part, there is outside intervention.\nAt first one cannot be sure of its meaning: is it rescue or is it destruction?\nA person in this situation must keep his mind alert and not withdraw into\nhimself with a sulky gesture of refusal, but must greet the new turn with\nrespect. Thus he ultimately escapes the danger, and all goes well. Even\nhappy turns of fortune often come in a form that at first seems strange\nto us.",
        }
      }
  },
  {
    .number = 6,
    .sk = 0b00111010,
    .name = "Conflict",
    .cm = "The upper trigram, whose image is heaven, has an upward movement;\nthe lower trigram, water, in accordance with its nature tends downward. Thus\nthe two halves move away from each other, giving rise to the idea of conflict.\nThe attribute of the Creative is strength, that of the Abysmal is danger,\nguile. Where cunning has force before it, there is conflict. A third\nindication of conflict, in terms of character, is presented by the combination\nof deep cunning within and fixed determination outwardly. A person of this\ncharacter will certainly be quarrelsome.",
    .jd = "CONFLICT. You are sincere\nAnd are being obstructed.\nA cautious halt halfway brings good fortune.\nGoing through to the end brings misfortune.\nIt furthers one to see the great man.\nIt does not further one to cross the great water.",
    .j_cm = "Conflict develops when one feels himself to be in the right and runs\ninto opposition. If one is not convinced of being in the right, opposition\nleads to craftiness or high-handed encroachment but not to open conflict.\nIf a man is entangled in a conflict, his only salvation lies in being\nso clear-headed and inwardly strong that he is always ready to come to\nterms by meeting the opponent halfway. To carry one the conflict to the\nbitter end has evil effects even when one is the right, because the enmity\nis then perpetuated. It is important to see the great man, that is, an impartial\nman whose authority is great enough to terminate the conflict amicably\nor assure a just decision. In times of strife, crossing the great water\nis to be avoided, that is, dangerous enterprises are not to be begun, because\nin order to be successful they require concerted unity of focus. Conflict\nwithin weakens the power to conquer danger without.",
    .im = "Heaven and water go their opposite ways:\nThe image of CONFLICT.\nThus in all his transactions the superior man\nCarefully considers the beginning.",
    .i_cm = "The image indicates that the causes of conflict are latent in the\nopposing tendencies of the two trigrams. Once these opposing tendencies\nappear, conflict is inevitable. To avoid it, therefore, everything must be\ntaken carefully into consideration in the very beginning. If rights and duties\nare exactly defined, or if, in a group, the spiritual trends of the individuals\nharmonize, the cause of conflict is removed in advance.",
        .lines = {
        {
          .pd ="If one does not perpetuate the affair,\nThere is a little gossip.\nIn the end, good fortune comes.",
          .cm = "While a conflict is in the incipient stage, the best thing To do is\nto drop the issue. Especially when the adversary is stronger, it is not\nadvisable to risk pushing the conflict to a decision. It may come to a\nslight dispute, but in the end all goes well.",
        },
        {
          .pd ="One cannot engage in conflict;\nOne returns home, gives way.\nThe people of his town,\nThree hundred households,\nRemain free of guilt.",
          .cm = "In a struggle with an enemy of superior strength, retreat is no disgrace.\nTimely withdrawal prevents bad consequences. If, out of a false sense\nof honor, a man allowed himself to be tempted into an unequal conflict,\nhe would be drawing down disaster upon himself. In such a case a wise and\nconciliatory attitude benefits the whole community, which will then not\nbe drawn into the conflict.",
        },
        {
          .pd ="To nourish oneself on ancient virtue induces perseverance.\nDanger. In the end, good fortune comes.\nIf by chance you are in the service of a king,\nSeek not works.",
          .cm = "This is a warning of the danger that goes with an expansive disposition.\nOnly that which has been honestly acquired through merit remains a permanent\npossession. It can happen that such a possession may be contested, but\nsince it is really one's own, one cannot be robbed of it. Whatever a man\npossesses through the strength of his own nature cannot be lost. If one\nenters the service of a superior, one can avoid conflict only by not seeking\nworks for the sake of prestige. It is enough if the work is done: let the\nhonor go to the other.",
        },
        {
          .pd ="One cannot engage in conflict.\nOne turns back and submits to fate,\nChanges one's attitude,\nAnd finds peace in perseverance.\nGood fortune.",
          .cm = "This refers to a person whose inner attitude at first lacks peace.\nHe does not feel content with his situation and would like to improve\nit through conflict. In contrast tot the situation of the nine in the second\nplace, he is dealing with a weaker opponent and might therefore succeed.\nBut he cannot carry on the fight, because, since right is not on his side,\nhe cannot justify the conflict to his conscience. Therefore he turns back\nand accepts his fate. He changes his mind and finds lasting peace in being\nat one with eternal law. This brings good fortune.",
        },
        {
          .pd ="To contend before him\nBrings supreme good fortune.",
          .cm = "This refers to an arbiter in a conflict who is powerful and just,\nand strong enough to lend weight to the right side. A dispute can be turned\nover to him with confidence. If one is in the right, one attains great good\nfortune.",
        },
        {
          .pd ="Even if by chance a leather belt is bestowed on one,'\nBy the end of a morning\nIt will have been snatched away three times.",
          .cm = "Here we have someone who has carried a conflict to the bitter end\nand has triumphed. He is granted a decoration, but his happiness does not\nlast. He is attacked again and again, and the result is conflict without\nend.",
        }
      }
  },
  {
    .number = 7,
    .sk = 0b00000010,
    .name = "The Army",
    .cm = "This hexagram is made up of the trigrams K'an, water, and K'un, earth,\nand thus it symbolizes the ground water stored up in the earth. In the\nsame way military strength is stored up in the mass of the people--invisible\nin times of peace but always ready for use as a source of power. The attributes\nof the two trigrams are danger inside and obedience must prevail outside.\nOf the individual lines, the one that controls the hexagram is the strong\nnine in the second place, to which the other lines, all yielding, are\nsubordinate. This line indicates a commander, because it stands in the\nmiddle of one of the two trigrams. But since it is in the lower rather\nthan the upper trigram, it represents not the ruler but the efficient general,\nwho maintains obedience in the army by his authority.",
    .jd = "THE ARMY. The army needs perseverance\nAnd a strong man.\nGood fortune without blame.",
    .j_cm = "An army is a mass that needs organization in order to become a fighting force.\nWithout strict discipline nothing can be accomplished, but this\ndiscipline must not be achieved by force. It requires a strong man who\ncaptures the hearts of the people and awakens their enthusiasm. In order\nthat he may develop his abilities he needs the complete confidence of his\nruler, who must entrust him with full responsibility as long as the war\nlasts. But war is always a dangerous thing and brings with it destruction\nand devastation. Therefore it should not be resorted to rashly but, like\na poisonous drug, should be used as a last recourse.",
    .im =   "In the middle of the earth is water:\nThe image of THE ARMY.\nThus the superior man increases his masses\nBy generosity toward the people.",
    .i_cm = "Ground water is invisibly present within the earth. In the same way\nthe military power of a people is invisibly present in the masses. When\ndanger threatens, every peasant becomes present in the masses. When danger\nthreatens, every peasant becomes a soldier; when the war ends, he goes\nback to his plow. He who is generous toward the people wins their love,\nand a people living under a mild rule becomes strong and powerful. Only\na people economically strong can be important in military power. Such power\nmust therefore be cultivated by improving the economic condition of the\npeople and by humane government. Only when there is this invisible bond\nbetween government and people, so that the people are sheltered by their\ngovernment as ground water is sheltered by the earth, is it possible to\nwage a victorious war.",
        .lines = {
        {
          .pd ="An army must set forth in proper order.\nIf the order is not good, misfortune threatens.",
          .cm = "At the beginning of a military enterprise, order is imperative. A\njust and valid cause must exist, and the obedience and coordination of the\ntroops must be well organized, otherwise the result is inevitably failure.",
        },
        {
          .pd ="In the midst of the army.\nGood fortune. No blame.\nThe king bestows a triple decoration.",
          .cm = "The leader should be in the midst of his army, in touch with it, sharing\ngood and bad with the masses he leads. This alone makes him equal to\nthe heavy demands made upon him. He needs also the recognition of the\nruler. The decorations he receives are justified, because there is no question\nof personal preferment here: the whole army, whose center he is, is honored\nin his person.",
        },
        {
          .pd ="Perchance the army carries corpses in the wagon.\nMisfortune.",
          .cm = "Here we have a choice of two explanations. One points to defeat because\nsomeone other than the chosen leader interferes with the command; the\nother is similar in its general meaning, but the expression, \"carries\ncorpses in the wagon,\" is interpreted differently. At burials and at sacrifices\nto the dead it was customary in China for the deceased to whom the sacrifice\nwas made to be represented by a boy of the family, who sat in the dead\nman's place and was honored as his representative. On the basis of this\ncustom the text is interpreted as meaning that a \"corpse boy\" is sitting\nin the wagon, or, in other words, that authority is not being exercised by\nthe proper leaders but has been usurped by others. Perhaps the whole difficulty\nclears up if it is inferred that there has been an error in copying. The\ncharacter fan, meaning \"all,\" may have been misread as shih, which means\n\"corpse.\" Allowing for this error, the meaning would be that if the multitude\nassumes leadership of the army (rides in the wagon), misfortune will ensue.",
        },
        {
          .pd ="The army retreats. No blame.",
          .cm = "In the face of a superior enemy, with whom it would be hopeless to\nengage in battle, an orderly retreat is the only correct procedure, because\nit will save the army from defeat and disintegration. It is by no means\na sign of courage or strength to insist upon engaging in a hopeless struggle\nregardless of circumstances.",
        },
        {
          .pd ="There is game in the field.\nIt furthers one to catch it.\nWithout blame.\nLet the eldest lead the army.\nThe younger transports corpses;\nThen perseverance brings misfortune.",
          .cm = "Game is in the field--it has left its usual haunts in the forest and\nis devastating the fields. This points to an enemy invasion. Energetic\ncombat and punishment are here thoroughly justified, but they must not\ndegenerate into a wild melee in which everyone fends for himself. Despite\nthe greatest degree of perseverance and bravery, this would lead to misfortune.\nThe army must be directed by an experienced leader. It is a matter of waging\nwar, not of permitting the mob to slaughter all who fall into their hands;\nif they do, defeat will be the result, and despite all perseverance there\nis danger of misfortune.",
        },
        {
          .pd ="The great prince issues commands,\nFounds states, vests families with fiefs.\nInferior people should not be employed.",
          .cm = "The war has ended successfully, victory is won, and the king divided\nestates and fiefs among his faithful vassals. But it is important that\ninferior people should not come into power. If they have helped, let them\nbe paid off with money, but they should not be awarded lands or the privileges\nof rulers, lest power be abused.",
        }
      }
  },
  {
    .number = 8,
    .sk = 0b00010000,
    .name = "Holding Together [union]",
    .cm = "The waters on the surface of the earth flow together wherever they\ncan, as for example in the ocean, where all the rivers come together.\nSymbolically this connotes holding together and the laws that regulate\nit. The same idea is suggested by the fact that all the lines of the hexagram\nexcept the fifth, the place of the ruler, are yielding. The yielding lines\nhold together because they are influenced by a man of strong will in the\nleading position, a man who is their center of union. Moreover, this strong\nand guiding personality in turn holds together with the others, finding\nin them the complement of his own nature.",
    .jd = "HOLDING TOGETHER brings good fortune.\nInquire of the oracle once again\nWhether you possess sublimity, constancy, and perseverance;\nThen there is no blame.\nThose who are uncertain gradually join.\nWhoever come too late\nMeets with misfortune.",
    .j_cm = "What is required is that we unite with others, in order that all may\ncomplement and aid one another through holding together. But such holding\ntogether calls for a central figure around whom other persons may unite.\nTo become a center of influence holding people together is a grave matter\nand fraught with great responsibility. It requires greatness of spirit,\nconsistency, and strength. Therefore let him who wishes to gather others\nabout him ask himself whether he is equal to the undertaking, for anyone\nattempting the task without a real calling for it only makes confusion\nworse than if no union at all had taken place. But when there is a real\nrallying point, those who at first are hesitant or uncertain gradually come\nin of their own accord. Late-comers must suffer the consequences, for in\nholding together the question of the right time is also important. Relationships\nare formed and firmly established according to definite inner laws. Common\nexperiences strengthen these ties, and he who comes too late to share in\nthese basic experiences must suffer for it if, as a straggler, he finds\nthe door locked. If a man has recognized the necessity for union and does\nnot feel strong enough to function as the center, it is his duty to become\na member of some other organic fellowship.",
    .im =  "On the earth is water:\nThe image of HOLDING TOGETHER.\nThus the kings of antiquity\nBestowed the different states as fiefs\nAnd cultivated friendly relations\nWith the feudal lords.",
    .i_cm = "Water fills up all the empty places on the earth and clings fast to\nit. The social organization of ancient China was based on this principle\nof the holding together of dependents and rulers. Water flows to unite\nwith water, because all parts of it are subject to the same laws. So too\nshould human society hold together through a community of interests that\nallows each individual to feel himself a member of a whole. The central\npower of a social organization must see to it that every member finds that\nhis true interest lies in holding together with it, as was the case in the\npaternal relationship between king and vassals in ancient China.",
        .lines = {
        {
          .pd ="Hold to him in truth and loyalty;\nThis is without blame.\nTruth, like a full earthen bowl\"\nThus in the end\nGood fortune comes from without.",
          .cm = "Fundamental sincerity is the only proper basis for forming relationships.\nThis attitude, symbolized by a full earthen bowl, in which the content\nis everything and the empty form nothing, shows itself not in clever words\nbut through the strength of what lies within the speaker. This strength\nis so great that it has power to attract good fortune to itself from without.",
        },
        {
          .pd ="Hold to him inwardly.\nPerseverance brings good fortune.",
          .cm = "If a person responds perseveringly and in the right way to the behests\nfrom above that summon him to action, his relations with others are intrinsic\nand he does not lose himself. But if a man seeks association with others\nas if he were an obsequious office hunter, he throws himself away. He\ndoes not follow the path of the superior man, who never loses his dignity.",
        },
        {
          .pd ="You hold together with the wrong people.",
          .cm = "We are often among people who do not belong to our own sphere. In\nthat case we must beware of being drawn into false intimacy through force\nof habit. Needless to say, this would have evil consequences. Maintaining\nsociability without intimacy is the only right attitude toward people, because\notherwise we should not be free to enter into relationship with people of\nour own kind later on.",
        },
        {
          .pd ="Hold to him outwardly also.\nPerseverance brings good fortune.",
          .cm = "Here the relations with a man who is the center of union are well\nestablished. Then we may, and indeed we should, show our attachment openly.\nBut we must remain constant and not allow ourselves to be led astray.",
        },
        {
          .pd ="Manifestation of holding together.\nIn the hunt the king uses beaters on three sides only\nAnd forgoes game that runs off in front.\nThe citizens need no warning.\nGood fortune.",
          .cm = "In the royal hunts of ancient China it was customary to drive up the\ngame from three sides, but on the fourth the animals had a chance to\nrun off. If they failed to do this they had to pass through a gate behind\nwhich the king stood ready to shoot. Only animals that entered here were\nshot; those that ran off in front were permitted to escape. This custom\naccorded with a kingly attitude; the royal hunter did not wish to turn\nthe chase into a slaughter, but held that the kill should consist only of\nthose animals which had so to speak voluntarily exposed themselves. There\nis depicted here a ruler, or influential man, to whom people are attracted.\nThose who come to him he accepts, those who do not come are allowed to go\ntheir own way. He invited none, flatters none--all come of their own free\nwill. In this way there develops a voluntary dependence among those who hold\nhim. They do not have to be constantly on their guard but may express their\nopinions openly. Police measures are not necessary, and they cleave to\ntheir ruler of their own volition. The same principle of freedom is valid\nfor life in general. We should not woo favor from people. If a man cultivates\nwithin himself the purity and the strength that are necessary for one\nwho is the center of a fellowship, those who are meant for him come of\ntheir own accord.",
        },
        {
          .pd ="He finds no head for holding together.\nMisfortune.",
          .cm = "The head is the beginning. If the beginning is not right, there is\nno hope of a right ending. If we have missed the right moment for union\nand go on hesitating to give complete and full devotion, we shall regret\nthe error when it is too late.",
        }
      }
  },
  {
    .number = 9,
    .sk = 0b00110111,
    .name = "The Taming Power of the Small",
    .cm = "This hexagram means the force of the small--the power of the shadowy--that\nrestrains, tames, impedes. A weak line in the fourth place, that of the\nminister, holds the five strong lines in check. In the Image it is the\nwind blowing across the sky. The wind restrains the clouds, the rising\nbreath of the Creative, and makes them grow dense, but as yet is not strong\nenough to turn them to rain.\nThe hexagram presents a configuration of circumstances in which\na strong element is temporarily held in leash by a weak element.\nIt is only through gentleness that this can have a successful outcome.",
    .jd = "THE TAMING POWER OF THE SMALL\nHas success.\nDense clouds, no rain from our western region.",
    .j_cm = "This image refers to the state of affairs in China at the time when\nKing Wên, who came originally from the west, was in the east at\nthe court of the reigning tyrant Chou Hsin. The moment for action on a\nlarge scale had not yet arrived. King Wên could only keep the tyrant\nsomewhat in check by friendly persuasion. Hence the image of many clouds,\npromising moisture and blessing to the land, although as yet no rain falls.\nThe situation is not unfavorable; there is a prospect of ultimate success,\nbut there are still obstacles in the way, and we can merely take preparatory\nmeasures. Only through the small means of friendly persuasion can we exert\nany influence. The time has not yet come for sweeping measures. However,\nwe may be able, to a limited extent, to act as a restraining and subduing\ninfluence. To carry out our purpose we need firm determination within and\ngentleness and adaptability in external relations.",
    .im =   "The wind drives across heaven:\nThe image of THE TAMING POWER OF THE SMALL.\nThus the superior man\nRefines the outward aspect of his nature.",
    .i_cm = "The wind can indeed drive the clouds together in the sky; yet, being\nnothing but air, without solid body, it does not produce great or lasting\neffects. So also an individual, in times when he can produce no great\neffect in the outer world, can do nothing except refine the expression\nof his nature in small ways.",
        .lines = {
        {
          .pd ="Return to the way.\nHow could there be blame in this?\nGood fortune.",
          .cm = "It lies in the nature of a strong man to press forward. In so doing\nhe encounters obstructions. Therefore he returns to the way suited to\nhis situation, where he is free to advance or to retreat. In the nature\nof things this will bring good fortune, for it is wise and reasonable not\nto try to obtain anything by force.",
        },
        {
          .pd ="He allows himself to be drawn into returning.\nGood fortune.",
          .cm = "One would like to press forward, but before going farther one sees\nfrom the example of others like oneself that this way is blocked. In\nsuch a case, if the effort to push forward is not in harmony with the\ntime, a reasonable and resolute man will not expose himself to a personal\nrebuff, but will retreat with others of like mind. This brings good fortune,\nbecause he does not needlessly jeopardize himself.",
        },
        {
          .pd ="The spokes burst out of the wagon wheels.\nMan and wife roll their eyes.",
          .cm = "Here an attempt is made to press forward forcibly, in the consciousness\nthat the obstructing power is slight. But since, under the circumstances,\npower actually lies with the weak, this sudden offensive is doomed to\nfailure. External conditions hinder the advance, just as loss of the wheel\nspokes stops the progress of a wagon. We do not yet heed this hint form\nfate, hence there are annoying arguments like those of a married couple.\nNaturally this is not a favorable state of thing, for though the situation\nmay enable the weaker side to hold its ground, the difficulties are too\nnumerous to permit of a happy result. In consequence even the strong man\ncannot so use his power as to exert the right influence on those around\nhim. He experiences a rebuff where he expected an easy victory, and he\nthus compromises his dignity.",
        },
        {
          .pd ="If you are sincere, blood vanishes and fear gives way.\nNo blame.",
          .cm = "If one is in the difficult and responsible position of counselor to\na powerful man, on should restrain him in such a way that the threat\nof actual bloodshed may arise. Nonetheless, the power of disinterested\ntruth is greater than all theses obstacles. It carries such weight that\nthe end is achieved, and all danger of bloodshed and all fear disappear.",
        },
        {
          .pd ="If you are sincere and loyally attached,\nYou are rich in your neighbor.",
          .cm = "Loyalty leads to firm ties because it means that each partner complements\nthe other. In the weaker person loyalty consists in devotion, in the\nstronger it consists in trustworthiness. This relation of mutual reinforcement\nleads to a true wealth that is all the more apparent because it is not\nselfishly hoarded but is shared with friends. Pleasure shared is pleasure\ndoubled.",
        },
        {
          .pd ="The rain comes, there is rest.\nThis is due to the lasting effect of character.\nPerseverance brings the woman into danger.\nThe moon is nearly full.\nIf the superior man persists,\nMisfortune comes.",
          .cm = "Success is at hand. The wind has driven up the rain. A fixed standpoint\nhas been reach. This has come about through the cumulation of small effects\nproduced by reverence for a superior character. But a success thus secured\nbit by bit calls for great caution. It would be a dangerous illusion\nfor anyone to think he could presume upon it. The female principle, the\nweak element that has won the victory, should never persist in vaunting\nit--that would lead to danger. The dark power in the moon is strongest\nwhen the moon is almost full. When it is full and directly opposite the\nsun, its waning is inevitable. Under such circumstances one must be content\nwith what has been achieved. To advance any further, before the appropriate\ntime has come, would lead to misfortune.",
        }
      }
  },
  {
    .number = 10,
    .sk = 0b00111011,
    .name = "Treading [conduct]",
    .cm = "The name of the hexagram means on the one hand the right way of conducting\noneself. Heaven, the father, is above, and the lake, the youngest daughter,\nis below. This shows the difference between high and low, upon which\ncomposure correct social conduct, depends. On the other hand the word\nfor the name of the hexagram, TREADING, means literally treading upon\nsomething. The small and cheerful [Tui] treads upon the large and strong\n[Ch'ien]. The direction of movement of the two primary trigrams is upward.\nThe fact that the strong treads on the weak is not mentioned in the Book\nof Changes, because it is taken for granted. For the weak to take a stand\nagainst the strong is not dangerous here, because it happened in good humor\n[Tui] and without presumption, so that the strong man is not irritated\nbut takes it all in good part.",
    .jd = "TREADING. Treading upon the tail of the tiger.\nIt does not bite the man. Success.",
    .j_cm = "The situation is really difficult. That which is strongest and that\nwhich is weakest are close together. The weak follows behind the strong\nand worries it. The strong, however, acquiesces and does not hurt the\nweak, because the contact is in goof humor and harmless. In terms of\na human situation, one is handling wild, intractable people. In such a\ncase one's purpose will be achieved if one behaves with decorum. Pleasant\nmanners succeed even with irritable people.",
    .im =   "Heaven above, the lake below:\nThe image of TREADING.\nThus the superior man discriminates between high and low,\nAnd thereby fortifies the thinking of the people.",
    .i_cm = "Heaven and the lake show a difference of elevation that inheres in\nthe natures of the two, hence no envy arises. Among mankind also there\nare necessarily differences of elevation; it is impossible to bring about\nuniversal equality. But it is important that differences in social rank\nshould not be arbitrary and unjust, for if this occurs, envy and class\nstruggle are the inevitable consequences. If, on the other hand, external\ndifferences in rank correspond with differences in inner worth, and if inner\nworth forms the criterion of external rank, people acquiesce and order\nreigns in society.",
        .lines = {
        {
          .pd ="Simple conduct. \nProgress without blame.",
          .cm = "The situation is one in which we are still not bound by any obligations\nof social intercourse. If our conduct is simple, we remain free of them\nWe can quietly follow our predilections as long as we are content and\nmake not demands on people. The meaning of the hexagram is not standstill\nbut progress. A man finds himself in an altogether inferior position at\nthe start. However, he has the inner strength that guarantees progress.\nIf he can be content with simplicity, he can make progress without blame.\nWhen a man is dissatisfied with modest circumstances, he is restless and\nambitious and tries to advance, not for the sake of accomplishing anything\nworth while, but merely in order to escape from lowliness and poverty\nby dint of his conduct. Once his purpose is achieved, he is certain to\nbecome arrogant and luxury-loving. Therefore blame attaches to his progress.\nOn the other hand, a man who is good at his work is content to behave simply.\nHe wishes to make progress in order to accomplish something. When he attains\nhis goal, he does something worth while, an all is well.",
        },
        {
          .pd ="Treading a smooth, level course.\nThe perseverance of a dark man\nBrings good fortune.",
          .cm = "The situation of a lonely sage is indicated here. He remains withdrawn\nfrom the bustle of life, seeks nothing, asks nothing of anyone, and travels\nthrough life unassailed, on a level road. Since he is content and does\nnot challenge fate, he remains free of entanglements.",
        },
        {
          .pd ="A one-eyed man is able to see,\nA lame man is able to tread.\nHe treads on the tail of the tiger.\nThe tiger bites the man.\nMisfortune.\nThus does a warrior act on behalf of his great prince.",
          .cm = "A one-eyed man can indeed see, but not enough for clear vision. A\nlame man can indeed treat, but not enough to make progress. If in spite of\nsuch defects a man considers himself strong and consequently exposes himself\nto danger, he is inviting disaster, for he is undertaking something beyond\nhis strength. This reckless way of plunging ahead, regardless of the adequacy\nof one's powers, can be justified only in the case of a warrior battling\nfor his prince.",
        },
        {
          .pd ="He treads on the tail of the tiger.\nCaution and circumspection\nLead ultimately to good fortune.",
          .cm = "This text refers to a dangerous enterprise. The inner power to carry\nit through is there, but this inner power is combined with hesitating\ncaution in one's external attitude. This line contrasts with the preceding\nline, which is weak within but outwardly presses forward. Here one is sure\nof ultimate success, which consists in achieving one's purpose, that is,\nin overcoming danger by going forward.",
        },
        {
          .pd ="Resolute conduct.\nPerseverance with awareness of danger.",
          .cm = "This refers to the ruler of the hexagram as a whole. One sees that\none has to be resolute in conduct. But at the same time one must remain\nconscious of the danger connected with such resoluteness, especially if\nit is to be persevered in. Only awareness of the danger makes success possible.",
        },
        {
          .pd ="Look to your conduct and weigh the favorable signs.\nWhen everything is fulfilled, supreme good fortune comes.",
          .cm = "The work is ended. If we want to know whether good fortune will follow,\nwe must look back upon our conduct and its consequences. If the effects\nare good, then good fortune is certain. No one knows himself. It is only\nby the consequences of his actions, by the fruit of his labors, that a\nman can judge what he is to expect.",
        }
      }
  },
  {
    .number = 11,
    .sk = 0b00000111,
    .name = "Peace",
    .cm = "The Receptive, which moves downward, stands above; the Creative, which\nmoves upward, is below. Hence their influences meet and are in harmony,\nso that all living things bloom and prosper. This hexagram belongs to\nthe first month (February-March), at which time the forces of nature prepare\nthe new spring.",
    .jd = "PEACE. The small departs,\nThe great approaches.\nGood fortune. Success.",
    .j_cm = "This hexagram denotes a time in nature when heaven seems to be on\nearth. Heaven has placed itself beneath the earth, and so their powers unite\nin deep harmony. Then peace and blessing descend upon all living things.\nIn the world of man it is a time of social harmony; those in high places\nshow favor to the lowly, and the lowly and inferior is an end to all feuds.\nIn side, at the center, in the key position, is the light principle; the\ndark principle is outside. Thus the light has a powerful influence, while\nthe dark is submissive. In this way each receives its due. When the good\nelements of society occupy a central position and are in control, the evil\nelements come under their influence and change for the better. When the spirit\nof heaven rules in man, his animal nature also comes under its influence\nand takes its appropriate place. The individual lines enter the hexagram\nfrom below and leave it again at the top. Here the small, weak, and evil\nelements are about to take their departure, while the great, strong, and\ngood elements are moving up. This brings good fortune and success.",
    .im =   "Heaven and earth unite: \nthe image of PEACE.\nThus the ruler\nDivides and completes the course of heaven and earth,\nAnd so aids the people.",
    .i_cm = "Heaven and earth are in contact and combine their influences, producing\na time of universal flowering and prosperity. This stream of energy must\nbe regulated by the ruler of men. It is done by a process of division.\nThus men divide the uniform flow of time into the seasons, according to\nthe succession of natural phenomena, and mark off infinite space by the\npoints of the compass. In this way nature in its overwhelming profusion\nof phenomena is bounded and controlled. One the other hand, nature must\nbe furthered in her productiveness. This is done by adjusting the products\nto the right time and the right place, which increases the natural yield.\nThis controlling and furthering activity of man in his relation to nature\nis the work on nature that rewards him.",
        .lines = {
        {
          .pd ="When ribbon grass is pulled up, the sod comes with it.\nEach according to his kind.\nUndertakings bring good fortune.",
          .cm = "In times of prosperity every able man called to fill an office draws\nlike minded people along with him, just as in pulling up ribbon grass\none always pulls up a bunch of it, because the stalks are connected by\ntheir roots. In such times, when it is possible to extend influence widely,\nthe mind of an able man is set upon going out into life and accomplishing\nsomething.",
        },
        {
          .pd ="Bearing with the uncultured in gentleness,\nFording the river with resolution,\nNot neglecting what is distant,\nNot regarding one's companions:\nThus one may manage to walk in the middle.",
          .cm = "In times of prosperity it is important and above all to possess enough\ngreatness of soul to bear with imperfect people. For in the hands of\na great master no material is unproductive; he can find use for everything.\nBut this generosity is by no means laxity or weakness. It is during times\nof prosperity especially that we must always be ready to risk even dangerous\nundertakings, such as the crossing of a river, if they are necessary.\nSo too we must not neglect what is distant but must attend scrupulously\nto everything. Factionalism and the dominance of cliques are especially\nto be avoided. Even if people of like mind come forward together, they ought\nnot to form a faction by holding together for mutual advantage; instead,\neach man should do is duty. Theses are four ways in which one can overcome\nthe hidden danger of a gradual slackening that always lurks in any time\nof peace. And that is how one finds the middle way for action.",
        },
        {
          .pd ="No plain not followed by a slope.\nNo going not followed by a return.\nHe who remains persevering in danger\nIs without blame.\nDo not complain about this truth;\nEnjoy the good fortune you still possess.",
          .cm = "Everything on earth is subject to change. Prosperity is followed by\ndecline: this is the eternal law on earth. Evil can indeed be held in\ncheck but not permanently abolished. It always returns. This conviction\nmight induct melancholy, but it should not; it ought only to keep us from\nfalling into illusion when good fortune comes to us. If we continue mindful\nof the danger, we remain persevering and make no mistakes. As long as a\nman's inner nature remains stronger and richer than anything offered by\nexternal fortune, as long as he remains inwardly superior to fate, fortune\nwill not desert him.",
        },
        {
          .pd ="He flutters down, not boasting of his wealth,\nTogether with his neighbor,\nGuileless and sincere.",
          .cm = "In times of mutual confidence, people of high rank come in close contact\nwith the lowly quite simply and without boasting of their wealth. This\nis not due to the force of circumstances but corresponds with their inmost\nsentiment. The approach is made quite spontaneously, because it is based\non inner conviction.",
        },
        {
          .pd ="The sovereign I\nGives his daughter in marriage.\nAnd supreme good fortune.",
          .cm = "The sovereign I is T'ang the Completer. By his decree the imperial\nprincesses, although higher in rank than their husbands, had to obey\nthem like all other wives. Here too we are shown a truly modest union\nof high and low that brings happiness and blessings.",
        },
        {
          .pd ="The wall falls back into the moat.\nUse no army now.\nMake your commands known within your own town.\nPerseverance brings humiliation.",
          .cm = "The change alluded to in the middle of the hexagram has begun to take\nplace. The wall of the town sinks back into the moat from which it was\ndug. The hour of doom is at hand. When matters have come to this pass,\nwe should submit to fate and not try to stave it off by violent resistance.\nThe one recourse left us is to hold our own within our intimate circle.\nShould we persevere in trying to resist the evil in the usual way, our\ncollapse would only be more complete, and humiliation would be the result.",
        }
      }
  },
  {
    .number = 12,
    .sk = 0b00111000,
    .name = "Standstill [Stagnation]",
    .cm = "This hexagram is the opposite of the preceding one. Heaven is above,\ndrawing farther and farther away, while the earth below sinks farther\ninto the depths. The creative powers are not in relation. It is a time\nof standstill and decline. This hexagram is linked with the seventh month\n(August-September), when the year has passed its zenith and autumnal decay\nis setting in.",
    .jd = "STANDSTILL. Evil people do not further\nThe perseverance of the superior man.\nThe great departs; the small approaches.",
    .j_cm = "Heaven and earth are out of communion and all things are benumbed.\nWhat is above has no relation to what is below, and on earth confusion\nand disorder prevail. The dark power is within, the light power is without.\nWeakness is within, harshness without. Within are the inferior, and without\nare the superior. The way of inferior people is in ascent; the way of\nsuperior people is one the decline. But the superior people do not allow\nthemselves to be turned from their principles. If the possibility of exerting\ninfluence is closed to them, they nevertheless remain faithful to their\nprinciples and withdraw into seclusion.",
    .im =   "Heaven and earth do not unite:\nThe image of STANDSTILL.\nThus the superior man falls back upon his inner worth\nIn order to escape the difficulties.\nHe does not permit himself to be honored with revenue.",
    .i_cm = "When, owing to the influence of inferior men, mutual mistrust prevails\nin public life, fruitful activity is rendered impossible, because the\nfundaments are wrong. Therefore the superior man knows what he must do\nunder such circumstances; he does not allow himself to be tempted by dazzling\noffers to take part in public activities. This would only expose him to\ndanger, since he cannot assent to the meanness of the others. He therefore\nhides his worth and withdraws into seclusion.",
        .lines = {
        {
          .pd ="When ribbon grass is pulled up, the sod comes with it.\nEach according to his kind.\nPerseverance brings good fortune and success.",
          .cm = "The text is almost the same as that of the first line of the preceding\nhexagram, but with a contrary meaning. In the latter a man is drawing\nanother along with him on the road to an official career; here a man is\ndrawing another with him into retirement form public life. This is why\nthe text says here, \"Perseverance brings good fortune and success,\" and\nnot \"Undertakings bring good fortune.\" If it becomes impossible to make\nour influence count, it is only by retirement that we spare ourselves humiliation.\nSuccess in a higher sense can be ours, because we know how to safeguard\nthe value of our personalities.",
        },
        {
          .pd ="They bear and endure;\nThis means good fortune for inferior people.\nThe standstill serves to help the great man to attain success.",
          .cm = "Inferior people are ready to flatter their superiors in a servile\nway. They would also endure the superior man if he would put an end to their\nconfusion. This is fortunate for them. But the great man calmly bears the\nconsequences of the standstill. He does not mingle with the crowd of the\ninferior; that is not his place. By his willingness to suffer personally\nhe insures the success of his fundamental principles.",
        },
        {
          .pd ="They bear shame.",
          .cm = "Inferior people who have risen to power illegitimately do not feel\nequal to the responsibility they have taken upon themselves. In their\nhearts they begin to be ashamed, although at first they do not show it\noutwardly. This marks a turn for the better.",
        },
        {
          .pd ="He who acts at the command of the highest\nRemains without blame.\nThose of like mind partake of the blessing.",
          .cm = "The time of standstill is nearing the point of change into its opposite.\nWhoever wishes to restore order must feel himself called to the task\nand have the necessary authority. A man who sets himself up a capable\nof creating order according to his own judgment could make mistakes and\nend in failure. But the man who is truly called to the task is favored\nby the conditions of the time, and all those of like mind will share in\nhis blessing.",
        },
        {
          .pd ="Standstill is giving way.\nGood fortune for the great man.\n\"What if it should fail, what if it should fail?\"\nIn this way he ties it to a cluster of mulberry shoots.",
          .cm = "The time undergoes a change. The right man, able to restore order,\nhas arrived. Hence \"good fortune.\" But such periods of transition are\nthe very times in which we must fear and tremble. Success is assured only\nthrough greatest caution, which asks always, \"What if it should fail?\"\nWhen a mulberry bush is cut down, a number of unusually strong shoots\nsprout from the roots. Hence the image of tying something to a cluster\nof mulberry shoots is used to symbolize the way of making success certain.\nConfucius says about this line:\nDanger arises when a man feels secure in his position. Destruction\nthreatens when a man seeks to preserve his worldly estate. Confusion\ndevelops when a man has put everything in order. Therefore the superior\nman does not forget danger in his security, not ruin when he is well established,\nnor confusion when his affairs are in order. In this way he gains personal\nsafety and is able to protect the empire.",
        },
        {
          .pd ="The standstill comes to an end.\nFirst standstill, then good fortune.",
          .cm = "The standstill does not last forever. However, it does not cease of\nits own accord; the right man is needed to end it. This is the difference\nbetween a state of peace and a state of stagnation. Continuous effort\nis necessary to maintain peace: left to itself it would change into stagnation\nand disintegration. The time of disintegration, however, does not change\nback automatically to a condition of peace and prosperity; effort must\nbe put forth in order to end it. This shows the creative attitude that\nman must take if the world is to be put in order.",
        }
      }
  },
  {
    .number = 13,
    .sk = 0b00111101,
    .name = "Fellowship with Men",
    .cm = "The image of the upper trigram Ch'ien is heaven, and that of the lower,\nLi, is flame. It is the nature of fire to flame up to the heaven. This\ngives the idea of fellowship. IT is the second line that, by virtue of\nits central character, unites the five strong lines around it. This hexagram\nforms a complement to Shih, THE ARMY 7. In the latter, danger is within\nand obedience without--the character of a warlike army, which,\nin order to hold together, needs one strong man among the many who are weak.\nHere, clarity is within\nand strength without--the character of a peaceful union of men, which, in\norder to hold together, needs one yielding nature among many firm persons.",
    .jd = "FELLOWSHIP WITH MEN in the open.\nSuccess.\nIt furthers one to cross the great water.\nThe perseverance of the superior man furthers.",
    .j_cm = "True fellowship among men must be based upon a concern that is universal.\nIt is not the private interests of the individual that create lasting\nfellowship among men, but rather the goals of humanity. That is why it\nis said that fellowship with men in the open succeeds. If unity of this\nkind prevails, even difficult and dangerous tasks, such as crossing the\ngreat water, can be accomplished. But in order to bring about this sort\nof fellowship, a persevering and enlightened leader is needed--a man with\nclear, convincing, and inspiring aims and the strength to carry them out.\n(The inner trigram means clarity; the outer, strength.)",
    .im =   "Heaven together with fire:\nThe image of FELLOWSHIP WITH MEN.\nThus the superior man organizes the clans\nAnd makes distinctions between things.",
    .i_cm = "Heaven has the same direction of movement as fire, yet it is different\nfrom fire. Just as the luminaries in the sky serve for the systematic\ndivision and arrangement of time, so human society and all things that\nreally belong together must be organically arranged. Fellowship should\nnot be a mere mingling of individuals or of things--that would be chaos,\nnot fellowship. If fellowship is to lead to order, there must be organization\nwithin diversity.",
        .lines = {
        {
          .pd ="Fellowship with men at the gate.\nNo blame.",
          .cm = "The beginning of union among people should take place before the door.\nAll are equally close to one another. No divergent aims have yet arisen,\nand one makes not mistakes. The basic principles of any kind of union\nmust be equally accessible to all concerned. Secret agreements bring misfortune.",
        },
        {
          .pd ="Fellowship with men in the clan.\nHumiliation.",
          .cm = "There is danger here of formation of a separate faction on the basis\nof personal and egotistic interests. Such factions, which are exclusive\nand, instead of welcoming all men, must condemn one group in order to\nunite the others, originate from low motives and therefore lead in the\ncourse of time to humiliation.",
        },
        {
          .pd ="He hides weapons in the thicket;\nHe climbs the high hill in front of it.\nFor three years he does not rise up.",
          .cm = "Here fellowship has changed about to mistrust. Each man distrusts\nthe other, plans a secret ambush, and seeks to spy on his fellow form afar.\nWe are dealing with an obstinate opponent whom we cannot come at by this\nmethod. Obstacles standing in the way of fellowship with others are shown\nhere. One has mental reservations for one's own part and seeks to take his\nopponent by surprise. This very fact makes one mistrustful, suspecting the\nsame wiles in his opponent and trying to ferret them out. The result is that\none departs further and further from true fellowship. The longer this goes\non, the more alienated one becomes.",
        },
        {
          .pd ="He climbs up on his wall; he cannot attack.\nGood fortune.",
          .cm = "Here the reconciliation that follows quarrel mover nearer. It is true\nthat there are still dividing walls on which we stand confronting one\nanother. But the difficulties are too great. We get into straits, and\nthis brings us to our senses. We cannot fight, and therein lies our good\nfortune.",
        },
        {
          .pd ="Men bound in fellowship first weep and lament,\nBut afterward they laugh.\nAfter great struggles they succeed in meeting.",
          .cm = "Two people are outwardly separated, but in their hearts they are united.\nThey are kept apart by their positions in life. Many difficulties and\nobstructions arise between them and cause them grief. But, remaining true\nto each other, the allow nothing to separate them, and although it costs\nthem a severe struggle to overcome the obstacles, they will succeed. When\nthey come together their sadness will change to joy. Confucius says of\nthis:\nLife leads the thoughtful man on a path of many windings.Now the course\nis checked, now it runs straight again.Here winged thoughts may pour\nfreely forth in words,There the heavy burden of knowledge must be shut\naway in silence.But when two people are at one in the inmost hearts,They\nshatter even the strength of iron or of bronze.And when two people understand\neach other in their inmost hearts,Their words are sweet and strong, like\nthe fragrance of orchids.",
        },
        {
          .pd ="Fellowship with men in the meadow.\nNo remorse.",
          .cm = "The warm attachment that springs from the heart is lacking here. We\nare by this time actually outside of fellowship with others. However,\nwe ally ourselves with them. The fellowship does not include all, but only\nthose who happen to dwell near one another. The meadow is the pasture at\nthe entrance to the town. At this stage, the ultimate goal of the union\nof mankind has not yet been attained, but we need not reproach ourselves.\nWe join the community without separate aims of our own.",
        }
      }
  },
  {
    .number = 14,
    .sk = 0b00101111,
    .name = "Possession in Great Measure",
    .cm = "The fire in heaven above shines far, and all things stand out in the\nlight and become manifest. The weak fifth line occupies the place of\nhonor and all the strong lines are in accord with it.All things come\nto the man who is modest and kind in a high position.",
    .jd = "POSSESSION IN GREAT MEASURE.\nSupreme success.",
    .j_cm = "The two trigrams indicate that strength and clarity unite. Possessions\ngreat measure is determined by fate and accords with the time. How is\nit possible that the weak line has power to hold the strong lines fast\nand to possess them? It is done by virtue of unselfish modesty. The time\nis favorable--a time of strength within, clarity and culture without. Power\nis expressing itself in graceful and controlled way. This brings supreme\nsuccess and wealth.",
    .im =   "Fire in heaven above:\nThe image of POSSESSION IN GREAT MEASURE.\nThus the superior man curbs evil and furthers good,\nAnd thereby obeys the benevolent will of heaven.",
    .i_cm = "The sun in heaven above, shedding light over everything one earth,\nis the image of possession on a grand scale. But a possession of this\nsort must be administered properly. The sun brings both evil and good\ninto the light of day. Man must combat and curb the evil, and must favor\nand promote the good. Only in this way does he fulfill the benevolent\nwill of God, who desires only good and not evil.",
        .lines = {
        {
          .pd ="No relationship with what is harmful;\nThere is no blame in this.\nIf one remains conscious of difficulty,\nOne remains without blame.",
          .cm = "Great possession that is still in its beginnings and that has not\nyet been challenged brings no blame, since there has been no opportunity\nto make mistakes. Yet there are many difficulties to be overcome. It is only\nby remaining conscious of theses difficulties that one can keep inwardly\nfree of possible arrogance and wastefulness, and thus in principle overcome\nall cause for blame.",
        },
        {
          .pd ="A big wagon for loading.\nOne may undertake something.\nNo blame.",
          .cm = "Great possession consists not only in the quantity of goods at one's\ndisposal, but first and foremost, in their mobility and utility, for\nthen they can be used in undertakings, and we remain free of embarrassment\nand mistakes. The big wagon, which will carry a heavy load and in which\none can journey farm means that there are at hand able helpers who give\ntheir support t and are equal to their task. One can load great responsibility\nupon such persons, and this is necessary in important undertakings.",
        },
        {
          .pd ="A prince offers it to the Son of Heaven.\nA petty man cannot do this.",
          .cm = "A magnanimous, liberal-minded man should not regard what he possesses\nas his exclusive personal property , but should place it at the disposal\nof the ruler or of the people at large. In so doing, he takes the right\nattitude toward his possession, which as private property can never endure.\nA petty man is incapable of this. He is harmed by great possessions, because\ninstead of sacrificing them, he would keep them for himself.",
        },
        {
          .pd ="He makes a difference\nBetween himself and his neighbor.\nNo blame.",
          .cm = "This characterizes the position of a man placed among rich and powerful\nneighbors. It is a dangerous position. He must look neither to the right\nnor to the left, and must shun envy and the temptation to vie with others.\nIn this way he remains free of mistakes.",
        },
        {
          .pd ="He whose truth is accessible, yet dignified,\nHas good fortune.",
          .cm = "The situation is very favorable. People are being won not by coercion\nbut by unaffected sincerity, so that they are attached to us in sincerity\nand truth. However, benevolence alone is not sufficient at the time of\nPOSSESSION IN GREAT MEASURE. For insolence might begin to spread. Insolence\nmust be kept in bounds by dignity; then good fortune is assured.",
        },
        {
          .pd ="He is blessed by heaven.\nGood fortune.\nNothing that does not further.",
          .cm = "In the fullness of possession and at the height of power, one remains\nmodest and gives honor to the sage who stands outside the affairs of\nthe world. By this means one puts oneself under the beneficent influence\ndescending form heaven, and all goes well. Confucius says of this line:\nTo bless means to help. Heaven helps the man who is devoted; men help\nthe man who is true. He who walks in truth and is devoted in his thinking,\nand furthermore reveres the worthy, is blessed by heaven. He has good\nfortune, and there is nothing that would not further.",
        }
      }
  },
  {
    .number = 15,
    .sk = 0b00000100,
    .name = "Modesty",
    .cm = "This hexagram is made up of the trigrams Kên, Keeping Still,\nmountain, and K'un. The mountain is the youngest son of the Creative,\nthe representative of heaven and earth. It dispenses the blessings of\nheaven, the clouds and rain that gather round its summit, and thereafter\nshines forth radiant with heavenly light. This shows what modesty is and\nhow it functions in great and strong men. K'un, the earth, stands above.\nLowliness is a quality of the earth: this is the very reason why it appears\nin this hexagram as exalted, by being placed above the mountain. This shows\nhow modesty functions in lowly, simple people: they are lifted up by it.",
    .jd = "MODESTY creates success.\nThe superior man carries things through.",
    .j_cm = "It is the law of heaven to make fullness empty and to make full what\nis modest; when the sun is at its zenith, it must, according to the law\nof heaven, turn toward its setting, and at its nadir it rises toward a\nnew dawn. In obedience to the same law, the moon when it is full begins\nto wane, and when empty of light it waxes again. This heavenly law works\nitself out in the fates of men also. It is the law of earth to alter the\nfull and to contribute to the modest. High mountains are worn down by the\nwaters, and the valleys are filled up. It is the law of fate to undermine\nwhat is full and to prosper the modest. And men also hate fullness and\nlove the modest. The destinies of men are subject to immutable laws that\nmust fulfill themselves. But man has it in his power to shape his fate,\naccording as his behavior exposes him to the influence of benevolent or\nof destructive forces. When a man holds a high position and is nevertheless\nmodest, he shines with the light of wisdom; if he is in a lowly position\nand is modest, he cannot be passed by. Thus the superior man can carry out\nhis work to the end without boasting of what he has achieved.",
    .im =   "Within the earth, a mountain:\nThe image of MODESTY.\nThus the superior man reduces that which is too much,\nAnd augments that which is too little.\nHe weighs things and makes them equal.",
    .i_cm = "The wealth of the earth in which a mountain is hidden is not visible\nto the eye, because the depths are offset by the height of the mountain.\nThus high and low competent each other and the result is the plain. Here\nan effect that it took a long time to achieve, but that in the end seems\neasy of accomplishment and self-evident, is used as the image of modesty.\nThe superior man does the same thing when he establishes order in the world;\nhe equalizes the extremes that are the source of social discontent and thereby\ncreates just and equable conditions.",
        .lines = {
        {
          .pd ="A superior man modest about his modesty\nMay cross the great water.\nGood fortune.",
          .cm = "A dangerous enterprise, such as the crossing of a great stream, is\nmade much more difficult if many claims and considerations have to be\ntaken into account. On the other hand, the task is easy if it is attended\nto quickly and simply. Therefore the unassuming attitude of mind that goes\nwith modesty fits a man to accomplish even difficult undertakings: he\nimposes no demands or stipulations but settles matters easily and quickly.\nWhere no claims are put forward, no resistances arise.",
        },
        {
          .pd ="Modesty that comes to expression. \nPerseverance brings good fortune.",
          .cm = "\"Out of the fullness of the heart the mouth speaketh.\" When a man's\nattitude of mind is so modest that this expresses itself in his outward\nbehavior, it is a source of good fortune to him. For the possibility of\nexerting a lasting influence arises of itself and no one can interfere.",
        },
        {
          .pd ="A superior man of modesty and merit\nCarries things to conclusion.\nGood fortune.",
          .cm = "This is the center of the hexagram, where its secret is disclosed.\nA distinguished name is readily earned by great achievements. If a man\nallows himself to be dazzled by fame, he will soon be criticized, and\ndifficulties will arise. If, on the contrary, he remains modest despite\nhis merit, he makes himself beloved and wins the support necessary for\ncarrying his work through to the end.",
        },
        {
          .pd ="Nothing that would not further modesty\nIn movement.",
          .cm = "Everything has its proper measure. Even modesty in behavior can be\ncarried too far. Here, however, it is appropriate, because the place\nbetween a worthy helper below and a kindly ruler above carries great\nresponsibility. The confidence of the man in superior place must not\nbe abused nor the merits of the man in inferior placed concealed. There\nare officials who indeed do not strive for prominence; they hide behind\nthe letter of ordinances, decline all responsibility, accept pay without\ngiving its equivalent in work, and bear empty titles. This is the opposite\nof what is meant here by modesty. In such a position, modesty is shown by\ninterest in one's work.",
        },
        {
          .pd ="No boasting of wealth before one's neighbor.\nIt is favorable to attack with force.\nNothing that would not further.",
          .cm = "Modesty is not to be confused with weak good nature that lets things\ntake their own course. When a man holds a responsible position, he must\nat times resort to energetic measures. In doing so he must not try to\nmake an impression by boasting of his superiority but must make certain\nof the people around him. The measures taken should be purely objective\nand in no way personally offensive. Thus modesty manifests itself even\nin severity.",
        },
        {
          .pd ="Modesty that comes to expression.\nIt is favorable to set armies marching\nTo chastise one's own city and one's country.",
          .cm = "A person who is really sincere in his modesty must make it show in\nreality. He must proceed with great energy in this. When enmity arises\nnothing is easier than to lay the blame on another. A weak man takes offense\nperhaps, and draws back, feeling self-pity; he thinks that it is modesty\nthat keeps him from defending himself. Genuine modesty sets one to creating\norder and inspires one to begin by disciplining one's own ego and one's\nimmediate circle. Only through having the courage to marshal one's armies\nagainst oneself, will something forceful really be achieved.",
        }
      }
  },
  {
    .number = 16,
    .sk = 0b00001000,
    .name = "Enthusiasm",
    .cm = "The strong line in the fourth place, that of the leading official,\nmeets with response and obedience from all the other lines, which are\nall weak. The attribute of the upper trigram, Chên, is movement;\nthe attributes of K'un, the lower, are obedience and devotion. This begins\na movement that meets with devotion and therefore inspires enthusiasm,\ncarrying all with it. Of great importance, furthermore, is the law of movement\nalong the line of least resistance, which in this hexagram is enunciated\nas the law for natural events and for human life.",
    .jd = "ENTHUSIASM. \nIt furthers one to install helpers\nAnd to set armies marching.",
    .j_cm = "The time of ENTHUSIASM derives from the fact that there is at hand\nan eminent man who is in sympathy with the spirit of the people and acts\nin accord with it. Hence he finds universal and willing obedience. To\narouse enthusiasm it is necessary for a man to adjust himself and his\nordinances to the character of those whom he has to lead. The inviolability\nof natural laws rests on this principle of movement along the line of\nleast resistance. Theses laws are not forces external to things but represent\nthe harmony of movement immanent in them. That is why the celestial bodies\ndo not deviate from their orbits and why all events in nature occur with\nfixed regularity. It is the same with human society: only such laws are\nrooted in popular sentiment can be enforced, while laws violating this\nsentiment merely arouse resentment. Again, it is enthusiasm that enables\nus to install helpers for the completion of an undertaking without fear\nof secret opposition. It is enthusiasm too that can unify mass movements,\nas in war, so that they achieve victory.",
    .im =   "Thunder comes resounding out of the earth:\nThe image of ENTHUSIASM.\nThus the ancient kings made music\nIn order to honor merit,\nAnd offered it with splendor\nTo the Supreme Deity,\nInviting their ancestors to be present.",
    .i_cm = "When, at the beginning of summer, thunder--electrical energy--comes\nrushing forth from the earth again, and the first thunderstorm refreshes\nnature, a prolonged state of tension is resolved. Joy and relief make\nthemselves felt. So too, music has power to ease tension within the heart\nand to loosen the grip of obscure emotions. The enthusiasm of the heart\nexpresses itself involuntarily in a burst of song, in dance and rhythmic\nmovement of the body. From immemorial times the inspiring effect of the\ninvisible sound that moves all hearts, and draws them together, has mystified\nmankind. Rulers have made use of this natural taste for music; they elevated\nand regulated it. Music was looked upon as something serious and holy, designed\nto purify the feelings of men. It fell to music to glorify the virtues of\nheroes and thus to construct a bridge to the world of the unseen. In the\ntemple men drew near to God with music and pantomimes (out of this later\nthe theater developed). Religious feeling for the Creator of the world was\nunited with the most sacred of human feelings, that of reverence for the\nancestors. The ancestors were invited to these divine services as guests\nof the Ruler of Heaven and as representatives of humanity in the higher regions.\nThis uniting of the human past with the Divinity in solemn moments of religious\ninspiration established the bond between God and man. The ruler who revered\nthe Divinity in revering his ancestors became thereby the Son of Heaven,\nin whom the heavenly and the earthly world met in mystical contact. These\nideas are the final summation of Chinese culture. Confucius has said of the\ngreat sacrifice at which these rites were performed: \"He who could wholly\ncomprehend this sacrifice could rule the world as though it were spinning\non his hand.\"",
        .lines = {
        {
          .pd ="Enthusiasm that expresses itself\nBrings misfortune.",
          .cm = "A man in an inferior position has aristocratic connections about which\nhe boasts enthusiastically. This arrogance inevitably invites misfortune.\nEnthusiasm should never be an egotistic emotion; it is justified\nonly when it is a general feeling that unites one with others.",
        },
        {
          .pd ="Firm as a rock. \nNot a whole day.\nPerseverance brings good fortune.",
          .cm = "This describes a person who does not allow himself to be misled by\nany illusions. While others are letting themselves be dazzled by enthusiasm,\nhe recognizes with perfect clarity the first signs of the time. Thus\nhe neither flatters those above nor neglects those beneath him; he is\nas firm as a rock. When the first sign of discord appears, he knows the\nright moment for withdrawing and does not delay even for a day. Perseverance\nin such conduct will bring good fortune. Confucius says about this line:\nTo know the seeds, that is divine indeed. In his association with\nthose above him, the superior man does not flatter. In his association with\nthose beneath him, he is not arrogant. For he knows the seeds. The seeds\nare the first imperceptible beginning of movement, the first trace of good\nfortune (or misfortune) that shows itself. The superior man perceives the\nseeds and immediately takes actin. He does not wait even a whole day. In\nthe Book of Changes it is said: \"Firm as a rock. Not a whole day. Perseverance\nbrings good fortune.\"\nFirm as a rock, what need of a whole day?\nThe judgment can be known.\nThe superior man knows what is hidden and what is evident.\nHe knows weakness, he knows strength as well.\nHence the myriads look up to him.",
        },
        {
          .pd ="Enthusiasm that looks upward creates remorse.\nHesitation brings remorse.",
          .cm = "This line is the opposite of the preceding one: the latter bespeaks\nself-reliance, while here there is enthusiastic looking up to a leader.\nIf a man hesitates too long, this also will bring remorse. The right moment\nfor approach must be seized: only then will he do the right thing.",
        },
        {
          .pd ="The source of enthusiasm.\nHe achieves great things.\nDoubt not.\nYou gather friends around you\nAs a hair clasp gathers the hair.",
          .cm = "This describes a man who is able to awaken enthusiasm through his\nown sureness and freedom from hesitation. He attracts people because he has\nno doubts and is wholly sincere. Owing to his confidence in them he wins\ntheir enthusiastic co-operation and attains success. Just as a clasp draws\nthe hair together and hold it, so he draws man together by the support he\ngives them.",
        },
        {
          .pd ="Persistently ill, and still does not die.",
          .cm = "Here enthusiasm is obstructed. A man is under constant pressure, which\nprevents him from breathing freely. However, this pressure has its advantage--it\nprevents him from consuming his powers in empty enthusiasm. Thus constant\npressure can actually serve to keep one alive.",
        },
        {
          .pd ="Deluded enthusiasm.\nBut if after completion one changes,\nThere is no blame.",
          .cm = "It is a bad thing for a man to let himself be deluded by enthusiasm.\nBut if this delusion has run its course, and he is still capable of changing,\nthen he is freed of error. A sober awakening from false enthusiasm is\nquite possible and very favorable.",
        }
      }
  },
  {
    .number = 17,
    .sk = 0b00011001,
    .name = "Following",
    .cm = "The trigram Tui, the Joyous, whose attribute is gladness, is above;\nChên, the Arousing, which has the attribute of movement, is below.\nJoy in movement induces following. The Joyous is the youngest daughter,\nwhile the Arousing is the eldest son. An older man defers to a young girl\nand shows her consideration. By this he moves her to follow him.",
    .jd = "FOLLOWING has supreme success.\nPerseverance furthers. No blame.",
    .j_cm = "In order to obtain a following one must first know how to adapt oneself.\nIf a man would rule he must first learn to serve, for only in this way\ndoes he secure from those below him the joyous assent that is necessary\nif they are to follow him. If he has to obtain a following by force or\ncunning, by conspiracy or by creating faction, he invariably arouses resistance,\nwhich obstructs willing adherence. But even joyous movement can lead to\nevil consequences, hence the added stipulation, \"Perseverance furthers\"\n--that is, consistency in doing right-- together with \"No blame.\" Just as\nwe should not ask others to follow us unless this condition is fulfilled,\nso it is only under this condition that we can in turn follow others without\ncoming to harm. The thought of obtaining a following through adaptation\nto the demands of the time is a great and significant idea; this is why\nthe appended judgment is so favorable.",
    .im =   "Thunder in the middle of the lake:\nThe image of FOLLOWING.\nThus the superior man at nightfall\nGoes indoors for rest and recuperation.",
    .i_cm = "In the autumn electricity withdraws into the earth again and rests.\nHere it is the thunder in the middle of the lake that serves as the image--thunder\nin its winter rest, not thunder in motion. The idea of following in the\nsense of adaptation to the demands of the time grows out of this image.\nThunder in the middle of the lake indicates times of darkness and rest.\nSimilarly, a superior man, after being tirelessly active all day, allows\nhimself rest and recuperation at night. No situation can become favorable\nuntil one is able to adapt to it and does not wear himself out with mistaken\nresistance.",
        .lines = {
        {
          .pd ="The standard is changing.\nPerseverance brings good fortune.\nTo go out of the door in company\nProduces deeds.",
          .cm = "There are exceptional conditions in which the relation between leader\nand followers changes. It is implicit in the idea of following and adaptation\nthat if one wants to lead others, one must remain accessible and responsive\nto the views of those under him. At the same time, however, he must have\nfirm principles, so that he does not vacillate where there is only a question\nof current opinion. Once we are ready to listen to the opinions of others,\nwe must not associate exclusively with people who share our views or with\nmembers of our own party; instead, we must go out and mingle freely with\nall sorts of people, friends or foes. That is the only way to achieve something.",
        },
        {
          .pd ="If one clings to the little boy,\nOne loses the strong man.",
          .cm = "In friendships and close relationships an individual must make a careful\nchoice. He surrounds himself either with good or with bad company; he\ncannot have both at once. If he throws himself away on unworthy friends\nhe loses connection with people of intellectual power who could further\nhim in the good.",
        },
        {
          .pd ="If one clings to the strong man,\nOne loses the little boy.\nThrough following one finds what one seeks.\nIt furthers one to remain persevering.",
          .cm = "When the right connection with distinguished people has been found,\na certain loss naturally ensues. A man must part company with the inferior\nand superficial. But in his heart he will feel satisfied, because he\nseeks and needs for the development of his personality. The important\nthing is to remain firm. He must know what he wants and not be led astray\nby momentary inclinations.",
        },
        {
          .pd ="Following creates success.\nPerseverance brings misfortune.\nTo go one's way with sincerity brings clarity.\nHow could there be blame in this?",
          .cm = "It often happens, when a man exerts a certain amount of influence,\nthat he obtains a following by condescension toward inferiors. But the\npeople who attach themselves to him are not honest in their intentions.\nThey seek personal advantage and try to make themselves indispensable\nthrough flattery and subservience. If one becomes accustomed to such satellites\nand cannot do without them, it brings misfortune. Only when a man is completely\nfree from his ego, and intent, by conviction, upon what is right and essential,\ndoes he acquire the clarity that enables him to see through such people,\nand become free of blame.",
        },
        {
          .pd ="Sincere in the good. Good fortune.",
          .cm = "Every man must have something he follows--something that serves him\nas a lodestar. He who follows with conviction the beautiful and the good\nmay feel himself strengthened by this saying.",
        },
        {
          .pd ="He meets with firm allegiance\nAnd is still further bound.\nThe king introduces him\nTo the Western Mountain.",
          .cm = "This refers to a man, an exalted sage, who has already put the turmoil\nof the world behind him. But a follower appears who understands him and\nis not to be put off. So the sage comes back into the world and aids the\nother in his work. Thus there develops an eternal tie between the two.\nThe allegory is chosen from the annals of the Chou dynasty. The rulers\nof this dynasty honored men who had served them well by awarding them a\nplace in the royal family's temple of ancestors on the Western Mountain.\nIn this way they were regarded as sharing in the destiny of the ruling family.",
        }
      }
  },
  {
    .number = 18,
    .sk = 0b00100110,
    .name = "Work on what has been spoiled [ Decay ]",
    .cm = "The Chinese character ku represents a bowl in whose contents worms\nare breeding. This means decay. IT is come about because the gentle indifference\nin the lower trigram has come together with the rigid inertia of the\nupper, and the result is stagnation. Since this implies guilt, the conditions\nembody a demand for removal of the cause. Hence the meaning of the hexagram\nis not simply \"what has been spoiled\" but \"work on what has been spoiled\".",
    .jd = "WORK ON WHAT HAS BEEN SPOILED\nHas supreme success.\nIt furthers one to cross the great water.\nBefore the starting point, three days.\nAfter the starting point, three days.",
    .j_cm = "What has been spoiled through man's fault can be made good again through\nman's work. IT is not immutable fate, as in the time of STANDSTILL, that\nhas caused the state of corruption, but rather the abuse of human freedom.\nWork toward improving conditions promises well, because it accords the\npossibilities of the time. We must not recoil from work and danger-symbolized\nby crossing of the great water-but must take hold energetically. Success\ndepends, however, on proper deliberation. This is expressed by the lines,\n\"Before the starting point, three days. After the starting point, three\ndays.\" We must first know the cause of corruption before we can do away\nwith them; hence it is necessary to be cautious during the time before\nthe start. Then we must see to it that the new way is safely entered upon,\nso that a relapse may be avoided; therefore we must pay attention to the\ntime after the start. Decisiveness and energy must take the place of inertia\nand indifference that have led to decay, in order that the ending may be\nfollowed by a new beginning.",
    .im =   "The wind blows low on the mountain:\nThe image of DECAY.\nThus the superior man stirs up the people\nAnd strengthens their spirit.",
    .i_cm = "When the wind blow s slow on the mountain, it is thrown back and spoils\nthe vegetation. This contains a challenge to improvement. It is the same\nwith debasing attitudes and fashions; they corrupt human society. His\nmethods likewise must be derived from the two trigrams, but in such a\nway that their effects unfold in orderly sequence. The superior must first\nremove stagnation by stirring up public opinion, as the wind stirs up everything,\nand must strengthen and tranquilize the character of the people, as the\nmountain gives tranquillity and nourishment to all that grows in its vicinity.",
        .lines = {
        {
          .pd ="Setting right what has been spoiled by the father.\nIf there is a son,\nNo blame rests upon the departed father.\nDanger. In the end good fortune.",
          .cm = "Rigid adherence to tradition has resulted in decay. But the decay\nhas not yet penetrated deeply and so can still be easily remedied. It is\nas if a son were compensated for the decay his father allowed to creep in.\nThen no blame attaches to the father. However, one must not overlook the\ndanger or take the matter too lightly. Only if one is conscious of the danger\nconnected with every reform will everything go well in the end.",
        },
        {
          .pd ="Setting right what has been spoiled by the mother.\nOne must not be too persevering.",
          .cm = "This refers to mistakes that as a result of weakness have brought\nabout decay-hence the symbol, \"what has been spoiled by the mother. \" In\nsetting things right in such a case, a certain gentle consideration is called\nfor. In order not to wound, one should not attempt to proceed too drastically.",
        },
        {
          .pd ="Setting right what has been spoiled by the father.\nThere will be a little remorse. No great blame.",
          .cm = "This describes a man who proceeds a little too energetically in righting\nthe mistakes of the past. Now and then, as a result, minor discourse\nand annoyances will surely develop. But too much energy is better than\ntoo little. Therefore, although he may at times have slight cause for regret,\nhe remains free of any serious blame.",
        },
        {
          .pd ="Tolerating what has been spoiled by the father.\nIn continuing one sees humiliation.",
          .cm = "This shows the situation of someone too weak to take measures against\ndecay that has its roots in the past and is just beginning to manifest\nitself. It is allowed to run its course. If this continues, humiliation\nwill result.",
        },
        {
          .pd ="Setting right what has been spoiled by the father.\nOne meets with praise.",
          .cm = "An individual is confronted with corruption originating from neglect\nin former times. He lacks the power to ward it off alone, but with able\nhelpers he can at least bring about a thorough reform, if he cannot create\na new beginning, and this also is praiseworthy.",
        },
        {
          .pd ="He does not serve kings and princes,\nSets himself higher goals.",
          .cm = "Not every man has an obligation to mingle in the affairs of the world.\nThere are some who are developed to such a degree that they are justified\nin letting the world go its own way and refusing to enter public life\nwith a view to reforming it. But this does not imply a right to remain\nidle or to sit back and merely criticize. Such withdrawal is justified\nonly when we strive to realize in ourselves the higher aims of mankind.\nFor although the sage remains distant from the turmoil of daily life, he\ncreates incomparable human values for the future.",
        }
      }
  },
  {
    .number = 19,
    .sk = 0b00000011,
    .name = "Approach",
    .cm = "The Chinese word lin has a range of meanings that is not exhausted\nby any single word of another language. The ancient explanations in the\nBook of Changes give as its first meaning, \"becoming great.\" What becomes\ngreat are the two strong lines growing into the hexagram from below; the\nlight-giving power expands with them. The meaning is then further extended\nto include the concept of approach, especially the approach of what is\nlower. Finally the meaning includes the attitude of condescension of a\nman in high position toward the people, and in general the setting to work\non affairs. This hexagram is linked with the twelfth month (January-February),\nwhen after the winter solstice, the light power begins to ascend again.",
    .jd = "APPROACH has supreme success.\nPerseverance furthers.\nWhen the eighth month comes,\nThere will be misfortune.",
    .j_cm = "The hexagram as a whole points to a time of joyous, hopeful progress.\nSpring is approaching. Joy and forbearance bring high and low nearer\ntogether. Success is certain. But we must work with determination and\nperseverance to make full use of the propitiousness of the time. And on\nthing more: spring does not last forever. In the eighth month the aspects\nare reversed. Then only two strong, light lines are left; these do not\nadvance but are in retreat (see next hexagram). We must take heed of this\nchange in good time. If we meet evil before it becomes reality-before it\nhas even begun to stir-we can master it.",
    .im =   "The earth above the lake:\nThe image of APPROACH.\nThus the superior man is inexhaustible\nIn his will to teach,\nAnd without limits\nIn his tolerance and protection of the people.",
    .i_cm = "The earth borders upon the lake from above. This symbolizes the approach\nand condescension of the man of higher position to those beneath him.\nThe two parts of the image indicate what his attitude toward these people\nwill be. Just as the lake is inexhaustible in depth,\nso the sage is inexhaustible/nin his readiness to teach mankind, and just as the earth is boundlessly\nwide, sustaining and caring for all creatures on it, so the sage sustains\nand cares for all people and excludes no part of humanity.",
        .lines = {
        {
          .pd ="Joint approach.\nPerseverance brings good fortune.",
          .cm = "The good begins to prevail and to find response in influential circles.\nThis in turn is an incentive to men of ability. IT is well to join this\nupward trend, but we must not let ourselves be carried away by the current\nof the time; we must adhere perseveringly to what is right. This bring\ngood fortune.",
        },
        {
          .pd ="Joint approach.\nGood fortune.\nEverything furthers.",
          .cm = "When the stimulus to approach comes from a high place, and when a\nman has the inner strength and consistency that need no admonition, good\nfortune will ensue. Nor need the future cause any concern. He is well aware\nthat everything earthly is transitory, and that a descent follows upon every\nrise, but need not be confused by this universal law of fate. Everything\nserves to further. Therefore he will travel the paths of life swiftly, honestly,\nand valiantly.",
        },
        {
          .pd ="Comfortable approach.\nNothing that would further.\nIf one is induced to grieve over it,\nOne becomes free of blame.",
          .cm = "Things are going well for a man: he achieves power and influence.\nBut in this lies the danger that he may relax, and confident of his position,\nallow the easygoing, careless mood to show itself in his dealings with\nother people. This would inevitably be harmful. But there is possibility\nof a change of mood. If he regrets his mistaken attitude and feels the\nresponsibility of an influential position, he frees himself of faults.",
        },
        {
          .pd ="Complete approach.\nNo blame.",
          .cm = "While the three lower lines indicate rise to power and influence,\nthe three upper lines show the attitude of persons in higher position toward\nthose of lower rank for whom they procure influence. Here is shown the open-minded\napproach of a person of high rank to a man of ability whom he draws in\nto his own circle, regardless of class prejudice. This is very favorable.",
        },
        {
          .pd ="Wise approach.\nThis is right for a great prince.\nGood fortune.",
          .cm = "A prince, or anyone in a leading position, must have the wisdom to\nattract to himself people of ability who are expert in directing affairs.\nHis wisdom consists both in selecting the right people and in allowing\nthose chosen to have a free hand without interference from him. For only\nthrough such self-restraint will he find the experts needed to satisfy\nall of his requirements.",
        },
        {
          .pd ="Great hearted approach.\nGood-hearted approach.\nGood fortune. No blame.",
          .cm = "A sage who has put the world behind him and who in spirit has already\nwithdrawn from life may, under certain circumstances, decide to return\nonce more to the here and now and to approach other men. This means great\ngood fortune for the men whom he teaches and helps. And for him this great\nhearted humbling of himself is blameless.",
        }
      }
  },
  {
    .number = 20,
    .sk = 0b00110000,
    .name = "Contemplation (View)",
    .cm = "A slight variation of tonal stress gives the Chinese name for this\nhexagram a double meaning. It means both contemplating and being seen,\nin the sense of being an example. These ideas are suggested by the fact\nthat the hexagram can be understood as picturing a type of tower characteristic\nof ancient China.\nA tower of this kind commanded a wide view of the country; at the\nsame time, when situated on a mountain, it became a landmark that could be\nseen for miles around. Thus the hexagram shows a ruler who contemplates the\nlaw of heaven above him and the ways of the people below, and who, by means\nof good government, sets a lofty example to the masses. This hexagram is\nlinked with the eight month (September-October). The light-giving power retreats\nand the dark power is again on the increase. However, this aspect is\nnot material in the interpretation of the hexagram as a whole.",
    .jd = "CONTEMPLATION. The ablution has been made,\nBut not yet the offering.\nFull of trust they look up to him.",
    .j_cm = "The sacrificial ritual in China began with an ablution and a libation\nby which the Deity was invoked, after which the sacrifice was offered.\nThe moment of time between these two ceremonies is the most sacred of\nall, the moment of deepest inner concentration. If piety is sincere and\nexpressive of real faith, the contemplation of it has a transforming\nawe-spiring effect on those who witness it. Thus also in nature a holy\nseriousness is to be seen in the fact that natural occurrences are uniformly\nsubject to law. Contemplation of the divine meaning underlying the workings\nof the universe gives to the man who is called upon to influence others\nthe means of producing like effects. This requires that power of inner\nconcentration which religious contemplation develops in great men strong\nin faith. It enables them to apprehend the mysterious and divine laws of\nlife, and by means of profoundest inner concentration they give expression\nto these laws in their own persons. Thus a hidden spiritual power emanates\nfrom them, influencing and dominating others without their being aware of\nhow it happens.",
    .im =   "The wind blows over the earth:\nThe image of CONTEMPLATION.\nThus the kings of old visited the regions of the world,\nContemplated the people,\nAnd gave them instruction.",
    .i_cm = "When the wind blows over the earth it goes far and wide, and the grass\nmust bend to its power. These two occurrences find confirmation in the\nhexagram. The two images are used to symbolize a practice of the kings\nof old; in making regular journeys the ruler could, in the first place,\nsurvey his realm and make certain that none of the existing usages of the\npeople escaped notice; in the second, he could exert influence through which\nsuch customs as were unsuitable could be changed. All of this points to\nthe power possessed by a superior personality. On the one hand, such a man\nwill have a view of the real sentiments of the great mass of humanity and\ntherefore cannot be deceived; on the other, he will impress the people so\nprofoundly, by his mere existence and by the impact of his personality,\nthat they will be swayed by him as the grass by the wind.",
        .lines = {
        {
          .pd ="Boy like contemplation.\nFor an inferior man, no blame.\nFor a superior man, humiliation.",
          .cm = "This means contemplation from a distance, without comprehension. A\nman of influence is at hand, abut his influence is not understood by\nthe common people. This matters little in the case of the masses, for\nthey benefit by the actions of the ruling sage whether they understand\nthem or not. But for a superior man it is a disgrace. He must not content\nhimself with a shallow, thoughtless view of prevailing forces; he must\ncontemplate them as a connected whole and try to understand them.",
        },
        {
          .pd ="Contemplation through the crack of the door.\nFurthering for the perseverance of a woman.",
          .cm = "Through the crack of the door one has a limited outlook; one looks\noutward from within. Contemplation is subjectively limited. One tends\nto relate everything to oneself and cannot put oneself in another's place\nand understand his motives. This is appropriate for a good housewife. It\nis not necessary for her to be conversant with the affairs of the world.\nBut for a man who must take active part in public life, such a narrow, egotistic\nway of contemplating things is of course harmful.",
        },
        {
          .pd ="Contemplation of my life\nDecides the choice\nBetween advance and retreat.",
          .cm = "This is the place of transition. We no longer look outward to receive\npictures that are more or less limited and confused, but direct out contemplation\nupon ourselves in order to find a guideline for our decisions. This self-contemplation\nmeans the overcoming of naive egotism in the person who sees everything\nsolely form his own standpoint. He begins to reflect and in this way acquires\nobjectivity. However, self-knowledge does not mean preoccupation with\none's own thoughts; rather, it means concern about the effects one creates.\nIt is only the effects our lives produce that give us the right to judge\nwhether what we have done means progress or regression.",
        },
        {
          .pd ="Contemplation of the light of the kingdom.\nIt furthers one to exert influence as the guest of a king.",
          .cm = "This describes a man who understands the secrets by which a kingdom\ncan be made to flourish. Such a man must be given an authoritative position,\nin which he can exert influence. He should be, so to speak, a guest-that\nis, he should be honored and act independently, and should not be used\nas a tool.",
        },
        {
          .pd ="Contemplation of my life. \nThe superior man is without blame.",
          .cm = "A man in an authoritative position to whom others look up must always be ready\nfor self-examination.  The right sort of self-examination, however,\nconsists not in idle brooding over oneself\nbut in examining the effects one produces.\nOnly when these effects are good,\nand when one's influence on others is good, \nwill the contemplation of one's own life bring the satisfaction of knowing\noneself to be free of mistakes.",
        },
        {
          .pd ="Contemplation of his life.\nThe superior man is without blame.",
          .cm = "While the preceding line represents a man who contemplates himself,\nhere in the highest place everything that is personal, related to the\nego, is excluded. The picture is that of a sage who stands outside the\naffairs of the world. Liberated from his ego, he contemplates the laws\nof life and so realizes that knowing how to become free of blame is the\nhighest good.",
        }
      }
  },
  {
    .number = 21,
    .sk = 0b00101001,
    .name = "Biting Through",
    .cm = "This hexagram represents an open mouth (cf. hexagram\n27 with an obstruction (in the fourth place) between\nthe teeth. As a result the lips cannot meet. To bring them together one\nmust bite energetically through the obstacle. Since the hexagram is made\nup of the trigrams for thunder and for lightning, it indicates how obstacles\nare forcibly removed in nature.\nEnergetic biting through overcomes the obstacle that prevents joining\nof the lips; the storm with its thunder and lightning overcomes the disturbing\ntension in nature. Recourse to law and penalties overcomes the disturbances\nof harmonious social life caused by criminals and slanderers. The theme\nof this hexagram is a criminal lawsuit, in contradistinction to that of\nSung, CONFLICT 6, which refers to civil suits.",
    .jd = "BITING THROUGH has success.\nIt is favorable to let justice be administered.",
    .j_cm = "When an obstacle to union arises, energetic biting through brings\nsuccess. This is true in all situations. Whenever unity cannot be established,\nthe obstruction is due to a talebearer and traitor who is interfering and\nblocking the way. To prevent permanent injury, vigorous measures must be\ntaken at once. Deliberate obstruction of this sort does not vanish of its\nown accord. Judgment and punishment are required to deter or obviate it.\nHowever, it is important to proceed in the right way. The hexagram combines\nLi, clarity, and Chên, excitement. Li is yielding, Chên is hard.\nUnqualified hardness and excitement\nwould be too violent in meting out punishment;\nunqualified clarity and gentleness would be too weak.\nThe two together create\nthe just measure. It is of moment that the man who makes the decisions (represented\nby the fifth line) is gentle by nature, while he commands respect by\nhis conduct in his position.",
    .im =   "Thunder and lighting:\nThe image of BITING THROUGH.\nThus the kings of former times made firm the laws\nThrough clearly defined penalties.",
    .i_cm = "Penalties are the individual applications of the law. The lawsspecify\nthe penalties. Clarity prevails when mild and severepenalties are differentiated,\naccording to the nature of the crimes.This is symbolized by the clarity\nof lighting. The law is strengthenedby a just application of penalties.\nThis is symbolized by the terrorof thunder. This clarity and severity have\nthe effect of instillingrespect; it is not that the penalties are ends\nin themselves.The obstructions in the social life of man increase when\nthere is alack of clarity in the penal codes and slackness in executing\nthem.The only to strengthen the law is to make it clear and make penaltiescertain\nand swift.",
        .lines = {
        {
          .pd ="His feet are fastened in the stocks,\nSo that his toes disappear.\nNo blame.",
          .cm = "If a sentence is imposed the first time a man attempts to do wrong,\nthe penalty is a mild one. Only the toes are put in the stocks. This\nprevents him from sinning further and thus he becomes free of blame.\nIt is a warning to halt in time on the path of evil.",
        },
        {
          .pd ="Bites through tender meat,\nSo that his nose disappears.\nNo blame.",
          .cm = "It is easy to discriminate between right and wrong in this case;it\nis like biting through tender meat. But one encounters ahardened sinner,\nand, aroused by anger, one goes a little toofar. The disappearance of\nthe nose in the course of the bitesignifies that indignation blots out\nfiner sensibility. However,there is no great harm in this, because the\npenalty as such isjust.",
        },
        {
          .pd ="Bites on old dried meat\nAnd strikes on something poisonous.\nSlight humiliation. No blame.",
          .cm = "Punishment is to be carried out by someone who lacks the power and\nauthority to do so. Therefore the culprits do not submit. The matter\nat issue is an old one-as symbolized by salted game-and in dealing with\nit difficulties arise. This old meat is spoiled: by taking up the problem\nthe punisher arouses poisonous hatred against himself, and n this way is\nput in a somewhat humiliating position. But since punishment was required\nby the time, he remains free of blame.",
        },
        {
          .pd ="Bites on dried gristly meat.\nReceives metal arrows.\nIt furthers one to be mindful of difficulties\nAnd to be persevering.\nGood fortune.",
          .cm = "There are great obstacles to be overcome, powerful opponents are to\nbe punished. Though this is arduous, the effort succeeds. But it is necessary\nto be hard as metal and straight as an arrow to surmount the difficulties.\nIf one knows these difficulties and remains persevering, he attains good\nfortune. The difficult task is achieved in the end.",
        },
        {
          .pd ="Bites on dried lean meat.\nReceives yellow gold.\nPerseveringly aware of danger.\nNo blame.",
          .cm = "The case to be decided is indeed not easy but perfectly clear. Since\nwe naturally incline to leniency, we must make every effort to be like\nyellow gold-that is, as true as gold and as impartial as yellow, the color\nof the middle [the mean]. It is only by remaining conscious of the dangers\ngrowing out of the responsibility we have assumed that we can avoid making\nmistakes.",
        },
        {
          .pd ="His neck is fastened in the wooden cangue,\nSo that his ears disappear.\nMisfortune.",
          .cm = "In contrast to the first line, this line refers to a man who is incorrigible.\nHis punishment is the wooden cangue, and his ears disappear under it-that\nis to say, he is deaf to warnings. This obstinacy leads to misfortune.",
        }
      }
  },
  {
    .number = 22,
    .sk = 0b00100101,
    .name = "Grace",
    .cm = "This hexagram shows a fire that breaks out of the secret depths of\nthe earth and, blazing up, illuminates and beautifies the mountain, the\nheavenly heights. Grace-beauty of form-is necessary in any union if it\nis to be well ordered and pleasing rather than disordered and chaotic.",
    .jd = "GRACE has success.\nIn small matters\nIt is favorable to undertake something.",
    .j_cm = "Grace brings success. However, it is not the essential or fundamental\nthing; it is only the ornament and therefore be used sparingly and only\nin little things. In the lower trigram of fire a yielding line comes between\ntwo strong lines and makes them beautiful, but the strong lines are the\nessential content and the weak line is the beautifying form. In the upper\ntrigram of the mountain, the strong line takes the lead, so that here\nagain the strong element must be regarded as the decisive factor. In nature\nwe see in the sky the strong light of the sun; the life of the world depends\non it. But this strong, essential thing is changed and given pleasing variety\nby the moon and the stars. In human affairs, aesthetic form comes into\nbeing when traditions exist that, strong and abiding like mountains, are\nmade pleasing by a lucid beauty. By contemplating the forms existing in\nthe heavens we come to understand time and its changing demands. Through\ncontemplation of the forms existing in human society it becomes possible\nto shape the world.",
    .im =   "Fire at the foot of the mountain:\nThe image of GRACE.\nThus does the superior man proceed\nWhen clearing up current affairs.\nBut he dare not decide controversial issues in this way.",
    .i_cm = "The fire, whose light illuminates the mountain and makes it pleasing,\ndoes not shine far; in the same way, beautiful form suffices to brighten\nand to throw light upon matters of lesser moment, but important questions\ncannot be decided in this way. They require greater earnestness.",
        .lines = {
        {
          .pd ="He lends grace to his toes, \nleaves the carriage, and walks.",
          .cm = "A beginner in subordinate place must take upon himself the labor of\nadvancing. There might be an opportunity of surreptitiously easing the\nway-symbolized by the carriage-but a self-contained man scorns help gained\nin a dubious fashion. He thinks it more graceful to go on foot than to\ndrive in a carriage under false pretenses.",
        },
        {
          .pd ="Lends grace to the beard on his chin.",
          .cm = "The beard is not an independent thing; it moves only with the chin.\nThe image therefore means that form is to be considered only as a result\nand attribute of content. The beard is a superfluous ornament. To devote\ncare to it for its own sake, without regard for the inner content of which\nit is an ornament, would bespeak a certain vanity.",
        },
        {
          .pd ="Graceful and moist.\nConstant perseverance brings good fortune.",
          .cm = "This represents a very charming life situation. One is under the spell\nof grace and the mellow mood induced by wine. This grace can adorn, but\nit can also swamp us. Hence the warning not to sink into convivial indolence\nbut to remain constant in perseverance. Good fortune depends on this.",
        },
        {
          .pd ="Grace or simplicity?\nA white horse comes as if on wings.\nHe is not a robber,\nHe will woo at the right time.",
          .cm = "An individual is in a situation in which doubts arise as to which\nis better-to pursue the grace of external brilliance, or to return to simplicity.\nThe doubt itself implies the answer. Confirmation comes from the outside;\nit comes like a white winged horse. The white color indicates simplicity.\nAt first it may be disappointing to renounce the comforts that might\nhave been obtained, yet one finds peace of mind in a true relationship\nwith the friend who courts him. The winged horse is the symbol of the\nthoughts that transcend all limits of space and time.",
        },
        {
          .pd ="Grace in the hills and gardens.\nThe roll of silk is meager and small.\nHumiliation, but in the end good fortune.",
          .cm = "A man withdraws from contact with people of the lowlands, who seek\nnothing but magnificence and luxury, in to the solitude of the heights.\nThere he finds an individual to look up to, whom he would like to have\nas a friend. But the gifts he has to offer are poor and few, so that he\nfeels ashamed. However, it is not the material gifts that count, but sincerity\nof feeling, and so all goes well in the end.",
        },
        {
          .pd ="Simple grace. \nNo blame.",
          .cm = "Here at the highest stage of development all ornament is discarded.\nForm no longer conceals content but brings out its value to the full.\nPerfect grace consists not in exterior ornamentation of the substance,\nbut in the simple fitness of its form.",
        }
      }
  },
  {
    .number = 23,
    .sk = 0b00100000,
    .name = "Splitting Apart",
    .cm = "The dark lines are about to mount upward and overthrow the last firm,\nlight line by exerting a disintegrating influence on it. The inferior,\ndark forces overcome what is superior and strong, not by direct means,\nbut by undermining it gradually and imperceptibly, so that it finally collapses.\nThe lines of the hexagram present the image of a house, the top line\nbeing the roof, and because the roof is being shattered the house collapses.\nThe hexagram belongs to the ninth month (October-November). The yin power\npushes up ever more powerfully and is about to supplant the yang power\naltogether.",
    .jd = "SPLITTING APART. \nIt does not further one\nTo go anywhere.",
    .j_cm = "This pictures a time when inferior people are pushing forward and\nare about to crowd out the few remaining strong and superior men. Under these\ncircumstances, which are due to the time, it is not favorable for the\nsuperior man to undertake anything. The right behavior in such adverse\ntimes is to be deduced from the images and their attributes. The lower\ntrigram stands for the earth, whose attributes. The lower trigram stands\nfor the earth, whose attributes are docility and devotion. The upper trigram\nstands for the mountain, whose attribute is stillness. This suggests that\none should submit to the bad time and remain quiet. For it is a question\nnot of man's doing but of time conditions, which, according to the laws\nof heaven, show an alternation of increase and decrease, fullness and emptiness.\nIt is impossible to counteract these conditions of the time. Hence it is\nnot cowardice but wisdom to submit and avoid action.",
    .im =   "The mountain rests on the earth:\nThe image of SPLITTING APART.\nThus those above can ensure their position\nOnly by giving generously to those below.",
    .i_cm = "The mountain rests on the earth. When it is steep and narrow, lacking\na broad base, it must topple over. Its position is strong only when it\nrises out of the earth broad and great, not proud and steep. So likewise\nthose who rule rest on the broad foundation of the people. They too should\nbe generous and benevolent, like the earth that carries all. Then they\nwill make their position as secure as a mountain is in its tranquillity.",
        .lines = {
        {
          .pd ="The leg of the bed is split.\nThose who persevere are destroyed.\nMisfortune.",
          .cm = "Inferior people are on the rise and stealthily begin their destructive\nburrowing from below in order to undermine the place where the superior\nman rests. Those followers of the ruler who remain loyal are destroyed\nby slander and intrigue. The situation bodes disaster, yet there is nothing\nto do but wait.",
        },
        {
          .pd ="The bed is split at the edge.\nThose who persevere are destroyed.\nMisfortune.",
          .cm = "The power of the inferior people is growing. The danger draws close\nto one's person; already there are clear indication, and rest is disturbed.\nMoreover, in this dangerous situation one is as yet without help or friendly\nadvances from above or below. Extreme caution is necessary in this isolation.\nOne must adjust to the time and promptly avoid the danger. Stubborn perseverance\nin maintaining one's standpoint would lead to downfall.",
        },
        {
          .pd ="He splits with them. No blame.",
          .cm = "An individual finds himself in an evil environment to which he is\ncommitted by external ties. But he has an inner relationship with a superior\nman, and through this he attains the stability to free himself from the way\nof the inferior people around him. This brings him into opposition to them\nof course, but that is not wrong.",
        },
        {
          .pd ="The bed is split up to the skin.\nMisfortune.",
          .cm = "Here the disaster affects not only the resting place but even the\noccupant. No warning or other comment is added. Misfortune has reached its\npeak: it can no longer be warded off.",
        },
        {
          .pd ="A shoal of fishes. Favor comes through the court ladies.\nEverything acts to further.",
          .cm = "Here, in immediate proximity to the strong, light-giving principle\nat the top, the nature of the dark force undergoes a change. It no longer\nopposes the strong principle by means of intrigues but submits to its\nguidance. Indeed, as the head of the other weak lines, it leads all of\nthese to the strong line, just as a princess leads her maids-in-waiting\nlike a shoal of fishes to her husband and thus gains his favor. Inasmuch\nas the lower element thus voluntarily places itself under the higher, it\nattains happiness and the higher also receives its due. Therefore all goes\nwell.",
        },
        {
          .pd ="There is a large fruit still uneaten.\nThe superior man receives a carriage.\nThe house of the inferior man is split apart.",
          .cm = "Here the splitting apart reaches its end. When misfortune has spent\nitself, better times return. The seed of the good remains, and it is\njust when the fruit falls to the ground that food sprouts anew from its\nseed. The superior man again attains influence and effectiveness. He is\nsupported by public opinion as if in a carriage. But the inferior man's\nwickedness is visited upon himself. His house is split apart. A law of\nnature is at work here. Evil is not destructive to the good alone but inevitably\ndestroys itself as well. For evil, which lives solely by negation, cannot\ncontinue to exist on its own strength alone. The inferior man himself fares\nbest when held under control by a superior man.",
        }
      }
  },
  {
    .number = 24,
    .sk = 0b00000001,
    .name = "Return (The Turning Point)",
    .cm = "The idea of a turning point arises from the fact that after the dark\nlines have pushed all of the light lines upward and out of the hexagram,\nanother light line enters the hexagram from below. The time of darkness\nis past. The winter solstice brings the victory of light. This hexagram\nis linked with the eleventh month, the month of the solstice (December-January).",
    .jd = "RETURN. Success.\nGoing out and coming in without error.\nFriends come without blame.\nTo and fro goes the way.\nOn the seventh day comes return.\nIt furthers one to have somewhere to go.",
    .j_cm = "After a time of decay comes the turning point. The powerful light\nthat has been banished returns. There is movement, but it is not brought\nabout by force. The upper trigram K'un is characterized by devotion; thus\nthe movement is natural, arising spontaneously.\nFor this reason the transformation\nof the old becomes easy. The old is discarded and the new is introduced.\nBoth measures accord with the time; therefore no harm results. Societies\nof people sharing the same views are formed. But since these groups come\ntogether in full public knowledge and are in harmony with the time, all selfish\nseparatist tendencies are excluded, and no mistake is made. The idea of RETURN\nis based on the course of nature. The movement is cyclic, and the course\ncompletes itself. Therefore it is not necessary to hasten anything artificially.\nEverything comes of itself at the appointed time. This is the meaning of\nheaven and earth. All movements are accomplished in six stages, and the\nseventh brings return. Thus the winter solstice, with which the decline of\nthe year begins, comes in the seventh month after the summer solstice; so\ntoo sunrise comes in the seventh double hour after sunset. Therefore seven\nis the number of the young light, and it arises when six, the number of the\ngreat darkness, is increased by one. In this way the state of rest gives\nplace to movement."
        "Thunder within the earth:\nThe image of THE TURNING POINT.\nThus the kings of antiquity closed the passes\nAt the time of solstice.\nMerchants and strangers did not go about,\nAnd the ruler\nDid not travel through the provinces.",
    .im = "Thunder within the earth:\nThe image of THE TURNING POINT.\nThus the kings of antiquity closed the passes\nAt the time of solstice.\nMerchants and strangers did not go about,\nAnd the ruler\nDid not travel through the provinces.",
    .i_cm = "The winter solstice has always been celebrated in China as the resting\ntime of the year--a custom that survives in the time of rest observed\nat the new year. In winter the life energy, symbolized by thunder, the\nArousing, is still underground. Movement is just at its beginning; therefore\nit must be strengthened by rest so that it will not be dissipated by being\nused prematurely. This principle, i.e., of allowing energy that is renewing\nitself to be reinforced by rest, applies to all similar situations. The\nreturn of health after illness, the return of understanding after an estrangement:\neverything must be treated tenderly and with care at the beginning, so\nthat the return may lead to a flowering.",
        .lines = {
        {
          .pd ="Return from a short distance.\nNo need for remorse.\nGreat good fortune.",
          .cm = "Slight digressions from the good cannot be avoided, but one must turn\nback in time, before going too far. This is especially important in the\ndevelopment of character; every faintly evil thought must be put aside\nimmediately, before it goes too far and takes root in the mind. Then there\nis no cause for remorse, and all goes well.",
        },
        {
          .pd ="Quiet return. Good fortune.",
          .cm = "Return always calls for a decision and is an act of self-mastery.\nIt is made easier if a man is in good company. If he can bring himself to\nput aside pride and follow the example of good men, good fortune results.",
        },
        {
          .pd ="Repeated return. Danger. No blame.",
          .cm = "There are people of a certain inner instability who feel a constant\nurge to reverse themselves. There is danger in continually deserting\nthe good because of uncontrolled desires, then turning back to it again\nbecause of a better resolution. However, since this does not lead to habituation\nin evil, a general inclination to overcome the defect is not wholly excluded/",
        },
        {
          .pd ="Walking in the midst of others,\nOne returns alone.",
          .cm = "A man is in a society composed of inferior people, but is connected\nspiritually with a strong and good friend, and this makes him turn back\nalone. Although nothing is said of reward and punishment, this return\nis certainly favorable, for such a resolve to choose the good brings its\nown reward.",
        },
        {
          .pd ="Noblehearted return. No remorse.",
          .cm = "When the time for return has come, a man should not take shelter in\ntrivial excuses, but should look within and examine himself. And if he\nhas done something wrong he should make a noblehearted resolve to confess\nhis fault. No one will regret having taken this road.",
        },
        {
          .pd ="Missing the return. Misfortune.\nMisfortune from within and without.\nIf armies are set marching in this way,\nOne will in the end suffer a great defeat,\nDisastrous for the ruler of the country.\nFor ten years\nIt will not be possible to attack again.",
          .cm = "If a man misses the right time for return, he meets with misfortune.\nThe misfortune has its inner cause in a wrong attitude toward the world.\nThe misfortune coming upon him for without results from this wrong attitude.\nWhat is pictured here is blind obstinacy and the judgment that is visited\nupon it.",
        }
      }
  },
  {
    .number = 25,
    .sk = 0b00111001,
    .name = "Innocence (The Unexpected)",
    .cm = "Ch'ien, heaven is above; Chên, movement, is below. The lower\ntrigram Chên is under the influence of the strong line it has received\nform above, from heaven. When, in accord with this, movement follows\nthe law of heaven, man is innocent and without guile. His mind is natural\nand true, unshadowed by reflection or ulterior designs. For wherever conscious\npurpose is to be seen, there the truth and innocence of nature have been\nlost. Nature that is not directed by the spirit is not true but degenerate\nnature. Starting out with the idea of the natural, the train of thought\nin part goes somewhat further and thus the hexagram includes also the idea\nof the fundamental or unexpected.",
    .jd = "INNOCENCE. Supreme success.\nPerseverance furthers.\nIf someone is not as he should be,\nHe has misfortune,\nAnd it does not further him\nTo undertake anything.",
    .j_cm = "Man has received from heaven a nature innately good, to guide him\nin all his movements. By devotion to this divine spirit within himself, he\nattains an unsullied innocence that leads him to do right with instinctive\nsureness and without any ulterior thought of reward and personal advantage.\nThis instinctive certainty brings about supreme success and 'furthers through\nperseverance\". However, not everything instinctive is nature in this higher\nsense of the word, but only that which is right and in accord with the will\nof heaven. Without this quality of rightness, an unreflecting, instinctive\nway of acting brings only misfortune. Confucius says about this: \"He who\ndeparts from innocence, what does he come to? Heaven's will and blessing\ndo not go with his deeds.\"",
    .im =   "Under heaven thunder rolls:\nAll things attain the natural state of innocence.\nThus the kings of old,\nRich in virtue, and in harmony with the time,\nFostered and nourished all beings.",
    .i_cm = "In springtime when thunder, life energy, begins to move again under\nthe heavens, everything sprouts and grows, and all beings receive for\nthe creative activity of nature the childlike innocence of their original\nstate. So it is with the good rulers of mankind: drawing on the spiritual\nwealth at their command, they take care of all forms of life and all forms\nof culture and do everything to further them, and at the proper time.",
        .lines = {
        {
          .pd ="Innocent behavior brings good fortune.",
          .cm = "The original impulses of the heart are always good, so that we may\nfollow them confidently, assured of good fortune and achievement of our\naims.",
        },
        {
          .pd ="If one does not count on the harvest while plowing,\nNor on the use of the ground while clearing it,\nIt furthers one to undertake something.",
          .cm = "We should do every task for its own sake as time and place demand\nand not with an eye to the result. Then each task turns out well, and anything\nwe undertake succeeds.",
        },
        {
          .pd ="Undeserved misfortune.\nThe cow that was tethered by someone\nIs the wanderer's gain, the citizen's loss.",
          .cm = "Sometimes undeserved misfortune befalls a man at the hands of another,\nas for instance when someone passes by and takes a tethered cow along\nwith him. His gain is the owner's loss. In all transactions, no matter\nhow innocent, we must accommodate ourselves to the demands of the time,\notherwise unexpected misfortune overtakes us.",
        },
        {
          .pd ="He who can be persevering\nRemains without blame.",
          .cm = "We cannot lose what really belongs to us, even if we throw it away.\nTherefore we need have no anxiety. All that need concern us is that we\nshould remain true to our own natures and not listen to others.",
        },
        {
          .pd ="Use no medicine in an illness\nIncurred through no fault of your own.\nIt will pass of itself.",
          .cm = "An unexpected evil may come accidentally from without. If it does\nnot originate in one's own nature or have a foothold there, one should not\nresort to external means to eradicate it, but should quietly let nature take\nits course. Then improvement will come of itself.",
        },
        {
          .pd ="Innocent action brings misfortune.\nNothing furthers.",
          .cm = "When, in a given situation, the time is not ripe for further progress,\nthe best thing to do is to wait quietly, without ulterior designs. If\none acts thoughtlessly and tries to push ahead in opposition to fate,\nsuccess will not be achieved.",
        }
      }
  },
  {
    .number = 26,
    .sk = 0b00100111,
    .name = "The Taming Power of the Great",
    .cm = "The Creative is tamed by Kên, Keeping Still. This produces\ngreat power, a situation in contrast to that of the ninth hexagram, Hsiao\nCh'u, THE TAMING POWER OF THE SMALL, in which the Creative is tamed by the\nGentle alone. There one weak line must tame five strong lines, but here four\nstrong lines are restrained by two weak lines; in addition to a minister,\nthere is a prince, and the restraining power therefore is afar stronger.\nThe hexagram has a threefold meaning, expressing different aspects of the\nconcept \"Holding firm.\" Heaven within the mountain gives the idea of holding\nfirm in the sense of holding together; the trigram Kên which holds\nthe trigram ch'ien still, gives the idea of holding firm in the sense of\nholding back; the third idea is that of holding firm in the sense of caring\nfor and nourishing. This last is suggested by the fact that a strong line\nat the top, which is the ruler of the hexagram, is honored and tended as\na sage. The third of these meanings also attaches specifically to this strong\nline at the top, which represents the sage.",
    .jd = "THE TAMING POWER OF THE GREAT.\nPerseverance furthers.\nNot eating at home brings good fortune.\nIt furthers one to cross the great water.",
    .j_cm = "To hold firmly to great creative powers and store them up, as set\nforth in this hexagram, there is need of a strong, clear-headed man who is\nhonored by the ruler. The trigram Ch'ein points to strong creative power;\nKên indicates firmness and truth. Both point to light and clarity and\nto the daily renewal of character. Only through such daily self-renewal can\na man continue at the height of his powers. Force of habit helps to keep\norder in quiet times; but in periods when there is a great storing up of\nenergy, everything depends on the power of the personality. However, since\nthe worthy are honored, as in the case of the strong personality entrusted\nwith leadership by the ruler, it is an advantage not to eat at home but rather\nto earn one's bread by entering upon public office. Such a man is in harmony\nwith heaven; therefore even great and difficult undertakings, such as crossing\nthe great water, succeed."
        "Heaven within the mountain:\nThe image of THE TAMING POWER OF THE GREAT.\nThus the superior man acquaints himself with many sayings of antiquity\nAnd many deeds of the past,\nIn order to strengthen his character thereby.",
    .im = "Heaven within the mountain:\nThe image of THE TAMING POWER OF THE GREAT.\nThus the superior man acquaints himself with many sayings of antiquity\nAnd many deeds of the past,\nIn order to strengthen his character thereby.",
    .i_cm = "Heaven within the mountain points to hidden treasures. In the words\nand deeds of the past there lies hidden a treasure that men may use to\nstrengthen and elevate their own characters. The way to study the past\nis not to confine oneself to mere knowledge of history but, through application\nof this knowledge, to give actuality to the past.",
        .lines = {
        {
          .pd ="Danger is at hand. It furthers one to desist.",
          .cm = "A man wishes to make vigorous advance, but circumstances present an\nobstacle. He sees himself held back firmly. If he should attempt to fore\nan advance, it would lead him into misfortune. Therefore it is better\nfor him to compose himself and to wait until an outlet is offered for\nrelease of his stored-up energies.",
        },
        {
          .pd ="The axletrees are taken from the wagon.",
          .cm = "Here advance is checked just as in the third line of THE TAMING POWER\nOF THE SMALL 9. However, in the later the restraining force is\nslight; thus a conflict arises between the propulsive and the restraining\nmovement, as a result of which the spokes fall out of the wagon wheels,\nwhile here the restraining force is absolutely superior; hence no struggle\ntakes place. One submits and removes the axletrees from the wagon -in\nother words, contents himself with waiting. In this way energy accumulates\nfor a vigorous advance later on.",
        },
        {
          .pd ="A good horse that follows others.\nAwareness of danger,\nWith perseverance, furthers.\nPractice chariot driving and armed defense daily.\nIt furthers one to have somewhere to go.",
          .cm = "The way opens; the hindrance has been cleared away. A man is in contact\nwith a strong will acting in the same direction as his own, and goes\nforward like one good horse following another. But danger still threatens,\nand he must remain aware of it, or he will be robbed of his firmness.\nThus he must acquire skill on the one hand in what will take him forward,\nand on the other in what will protect him against unforeseen attacks. It\nis good in such a pass to have a goal toward which to strive.",
        },
        {
          .pd ="The headboard of a young bull.\nGreat good fortune.",
          .cm = "This line and the one following it are the two that tame the forward-pushing\nlower lines. Before a bull's horns grow out, a headboard is fastened\nto its forehead, so that later when the horns appear they cannot do harm.\nA good way to restrain wild force is to forestall it. By so doing one achieves\nan easy and great success.",
        },
        {
          .pd ="The tusk of a gelded boar.\nGood fortune.",
          .cm = "Here the restraining of the impetuous forward drive is achieved in\nan indirect way. A boar's tusk is in itself dangerous, but if the boar's\nnature is altered, the tusk is no longer a menace. Thus also where men\nare concerned, wild force should not be combated directly; instead, its\nroots should be eradicated.",
        },
        {
          .pd ="One attains the way of heaven.\nSuccess.",
          .cm = "The time of obstruction is past. The energy long dammed up by inhibition\nforces its way out and achieves great success. This refers to a man who\nis honored by the ruler and whose principles now prevail and shape the\nworld.",
        }
      }
  },
  {
    .number = 27,
    .sk = 0b00100001,
    .name = "Corners of the Mouth (Providing Nourishment)",
    .cm = "This hexagram is a picture of an open mouth; above and below are firm\nlines of the lips, and between them the opening. Starting with the mouth,\nthrough which we take food for nourishment, the thought leads to nourishment\nitself. Nourishment of oneself, specifically of the body, is represented\nin the three lower lines, while the three upper lines represent nourishment\nand care of others, in a higher, spiritual sense.",
    .jd = "THE CORNERS OF THE MOUTH.\nPerseverance brings good fortune.\nPay heed to the providing of nourishment\nAnd to what a man seeks\nTo fill his own mouth with.",
    .j_cm = "In bestowing care and nourishment, it is important that the right\npeople should be taken care of and that we should attend to our own nourishment\nin the right way. If we wish to know what anyone is like, we have only to\nobserve on whom he bestows his care\nand what sides of his own nature he cultivates and nourishes.\nNature nourishes all creatures. The great man fosters\nand takes care of superior men, in order to take care of all men through\nthem. Mencius says about this:\nIf we wish to know whether anyone is superior or not, we need only\nobserve what part of his being he regards as especially important. The\nbody has superior and inferior, important and unimportant parts. We must\nnot injure important parts for the sake of the unimportant, nor must we\ninjure the superior parts for the sake of the inferior. He who cultivates\nthe inferior parts of his nature is an inferior man. He who cultivates the\nsuperior parts of his nature is a superior man.",
    .im =   "At the foot of the mountain, thunder:\nThe image of PROVIDING NOURISHMENT.\nThus the superior man is careful of his words\nAnd temperate in eating and drinking.",
    .i_cm = "\"God comes forth in the sign of the Arousing\": when in the spring\nthe life forces stir again, all things comes into being anew. \"He brings\nto perfection in the sign of Keeping Still\": thus in the early spring, when\nthe seeds fall to earth, all things are made ready. This is an image of providing\nnourishment through movement and tranquillity. The superior man takes it\nas a pattern for the nourishment and cultivation of his character. Words\nare a movement going form within outward. Eating and drinking are movements\nfrom without inward. Both kinds of movement can be modified by tranquillity.\nFor tranquillity keeps the words that come out of the mouth from exceeding\nproper measure, and keeps the food that goes into the mouth from exceeding\nits proper measure. Thus character is cultivated.",
        .lines = {
        {
          .pd ="You let your magic tortoise go,\nAnd look at me with the corners of your mouth drooping.\nMisfortune.",
          .cm = "The magic tortoise is a creature possessed of such supernatural powers\nthat it lives on air and needs no earthly nourishment. The image means\nthat a man fitted by nature and position to live freely and independently\nrenounces this self-reliance and instead looks with envy and discontent\nat others who are outwardly in better circumstances. But such base envy\nonly arouses derision and contempt in those others. This has bad results.",
        },
        {
          .pd ="Turning to the summit for nourishment,\nDeviating from the path\nTo seek nourishment from the hill.\nContinuing to do this brings misfortune.",
          .cm = "Normally a person either provides his own means of nourishment or\nis supported in a proper way by those whose duty of privilege it is to provide\nfor him. If, owing to weakness of spirit, a man cannot support himself,\na feeling of uneasiness comes over him; this is because in shirking the\nproper way of obtaining a living, he accepts support as a favor from those\nin higher place. This is unworthy, for he is deviating from his true nature.\nKept up indefinitely, this course leads to misfortune.",
        },
        {
          .pd ="Turning away from nourishment.\nPerseverance brings misfortune.\nDo not act thus for ten years.\nNothing serves to further.",
          .cm = "He who seeks nourishment that does not nourish reels from desire to\ngratification and in gratification craves desire. Mad pursuit of pleasure\nfor the satisfaction of the senses never brings one to the goal. One\nshould never (ten years is a complete cycle of time) follow this path,\nfor nothing good can come of it.",
        },
        {
          .pd ="Turning to the summit\nFor provision of nourishment\nBrings good fortune.\nSpying about with sharp eyes\nLike a tiger with insatiable craving.\nNo blame.",
          .cm = "In contrast to the six in the second place, which refers to a man\nbent exclusively on his own advantage, this line refers to one occupying\na high position and striving to let his light sine forth. To do this he needs\nhelpers, because he cannot attain his lofty aim alone. With the greed of\na hungry tiger he is on the lookout for the right people. Since he is not\nworking for himself but for the good of all, there is no wrong in such zeal.",
        },
        {
          .pd ="Turning away from the path.\nTo remain persevering brings good fortune.\nOne should not cross the great water.",
          .cm = "A man may be conscious of a deficiency in himself. He should be undertaking\nthe nourishment of the people, but he has not the strength to do it.\nThus he must turn from his accustomed path and beg counsel and help from\na man who is spiritually his superior but undistinguished outwardly. If\nhe maintains this attitude of mind perseveringly, success and good fortune\nare his. But he must remain aware of his dependence. He must not put his\nown person forward nor attempt great labors, such as crossing the great\nwater.",
        },
        {
          .pd ="The source of nourishment.\nAwareness of danger brings good fortune.\nIt furthers one to cross the great water.",
          .cm = "This describes a sage of the highest order, from whom emanate all\ninfluences that provide nourishment for others. Such a position brings with\nit heavy responsibility. If he remains conscious of this fact, he has good\nfortune and may confidently undertake even great and difficult labors, such\nas crossing the great water. These undertakings bring general happiness for\nhim and for all others.",
        }
      }
  },
  {
    .number = 28,
    .sk = 0b00011110,
    .name = "Preponderance of the Great",
    .cm = "This hexagram consists of four strong lines inside and two weak lines\noutside. When the strong are outside and the weak inside, all is well\nand there is nothing out of balance, nothing extraordinary in the situation.\nHere, however, the opposite is the case. The hexagram represents a beam\nthat is thick and heavy in the middle but too weak at the ends. This is\na condition that cannot last; it must be changed, must pass, or misfortune\nwill result.",
    .jd = "PREPONDERANCE OF THE GREAT.\nThe ridgepole sags to the breaking point.\nIt furthers one to have somewhere to go.\nSuccess.",
    .j_cm = "The weight of the great is excessive. The load is too heavy for the\nstrength of the supports. The ridgepole on which the whole roof rests,\nsags to the breaking point, because its supporting ends are too weak for\nthe load they bear. It is an exceptional time and situation; therefore\nextraordinary measures are demanded. It is necessary to find a way of\ntransition as quickly as possible, and to take action. This promises success.\nFor although the strong element is in excess, it is in the middle, that\nis, at the center of gravity, so that a revolution is not to be feared.\nNothing is to be achieved by forcible measures. The problem must be solved\nby gently penetration to the meaning of the situation (as is suggested\nby the attribute of the inner trigram, Sun); then the change-over to other\nconditions will be successful. It demands real superiority; therefore\nthe time when the great preponderates is a momentous time.",
    .im =   "The lake rises above the trees:\nThe image of PREPONDERANCE OF THE GREAT.\nThus the superior man, when he stands alone,\nIs unconcerned,\nAnd if he has to renounce the world,\nHe is undaunted.",
    .i_cm = "Extraordinary times when the great preponderates are like flood times\nwhen the lake rises over the treetops. But such conditions are temporary.\nThe two trigrams indicate the attitude proper to such exceptional times:\nthe symbol of the trigram Sun is the tree, which stands firm even though\nit stands alone, and the attribute of Tui is joyousness, which remains\nundaunted even if it must renounce the world.",
        .lines = {
        {
          .pd ="To spread white rushes underneath.\nNo blame.",
          .cm = "When a man wishes to undertake an enterprise in extraordinary times,\nhe must be extraordinarily cautious, just as when setting a heavy thing\ndown on the floor, one takes care to put rushes under it, so that nothing\nwill break. This caution, though it may seem exaggerated, is not a mistake.\nExceptional enterprises cannot succeed unless utmost caution is observed\nin their beginnings and in the laying of their foundations.",
        },
        {
          .pd ="A dry poplar sprouts at the root.\nAn older man takes a young wife.\nEverything furthers.",
          .cm = "Wood is near water; hence the image of an old poplar sprouting at\nthe root. This means an extraordinary situation arises when an older man\nmarries a young girl who suits him. Despite the unusualness of the situation,\nall goes well. From the point of view of politics, the meaning is that in\nexceptional times one does well to join with the lowly, for this affords\na possibility of renewal.",
        },
        {
          .pd ="The ridgepole sags to the breaking point.\nMisfortune.",
          .cm = "This indicates a type of man who in times of preponderance of the\ngreat insists on pushing ahead. He accepts no advice from others, and therefore\nthey in turn are not willing to lend him support. Because of this the\nburden grows, until the structure of things bends or breaks. Plunging\nwillfully ahead in times of danger only hastens the catastrophe.",
        },
        {
          .pd ="The ridgepole is braced. Good fortune.\nIf there are ulterior motives, it is humiliating.",
          .cm = "Through friendly relations with people of lower rank, a responsible\nman succeeds in becoming master of the situation. But if, instead of\nworking for the rescue of the whole, he were to misuse his connections\nto obtain personal power and success, it would lead to humiliation.",
        },
        {
          .pd ="A withered poplar puts forth flowers.\nAn older woman takes a husband.\nNo blame. No praise.",
          .cm = "A withered poplar that flowers exhausts its energies thereby and only\nhastens its end. An older woman may marry once more, but no renewal takes\nplace. Everything remains barren. Thus, though all the amenities are\nobserved, the net result is only the anomaly of the situation. Applied\nto politics, the metaphor means that if in times of insecurity we give\nup alliance with those below us and keep up only the relationships we\nhave with people of higher rank, an unstable situation is created.",
        },
        {
          .pd ="One must go through the water.\nIt goes over one's head.\nMisfortune. No blame.",
          .cm = "Here is a situation in which the unusual has reached a climax. One\nis courageous and wishes to accomplish one's task, no matter what happens.\nThis leads into danger. The water rises over one's head. This is the\nmisfortune. But one incurs no blame in giving up one's life that the\ngood and the right may prevail. There are things that are more important\nthan life.",
        }
      }
  },
  {
    .number = 29,
    .sk = 0b00010010,
    .name = "The Abysmal (Water)",
    .cm = "This hexagram consists of a doubling of the trigram K'an. It is one\nof the eight hexagrams in which doubling occurs. The trigram K'an means\na plunging in. A yang line has plunged in between two yin lines and is\nclosed in by them like water in a ravine. The trigram K'an is also the\nmiddle son. The Receptive has obtained the middle line of the Creative,\nand thus K'an develops. As an image it represents water, the water that\ncomes from above and is in motion on earth in streams and rivers, giving\nrise to all life on earth. In man's world K'an represents the heart,\nthe soul locked up within the body,\nthe principle of light enclosed in the dark--that is, reason.\nThe name of the hexagram, because the trigram is doubled, has\nthe additional meaning, \"repetition of danger.\" Thus the hexagram is intended\nto designate an objective situation to which one must become accustomed,\nnot a subjective attitude. For danger due to a subjective attitude means\neither foolhardiness or guile. Hence too a ravine is used to symbolize danger;\nit is a situation in which a man is in the same pass as the water in a ravine,\nand, like the water, he can escape if he behaves correctly.",
    .jd = "The Abysmal repeated.\nIf you are sincere, you have success in your heart,\nAnd whatever you do succeeds.",
    .j_cm = "Through repetition of danger we grow accustomed to it. Water sets\nthe example for the right conduct under such circumstances. It flows on and\non, and merely fills up all the places through which it flows; it does not\nshrink from any dangerous spot nor from any plunge, and nothing can make\nit lose its own essential nature.\nIt remains true to itself under all conditions.\nThus likewise, if one is sincere when confronted with difficulties,\nthe heart can penetrate the meaning of the situation. And once we have gained\ninner mastery of a problem, it will come about naturally that the action\nwe take will succeed. In danger all that counts is really carrying out\nall that has to be done--thoroughness--and going forward, in order not\nto perish through tarrying in the danger. Properly used, danger can have\nan important meaning as a protective measure. Thus heaven has its perilous\nheight protecting it against every attempt at invasion, and earth has its\nmountains and bodies of water, separating countries by their dangers.\nThus also rulers make use of danger to protect themselves against attacks\nfrom without and against turmoil within.",
    .im =   "Water flows on uninterruptedly and reaches its foal:\nThe image of the Abysmal repeated.\nThus the superior man walks in lasting virtue\nAnd carries on the business of teaching.",
    .i_cm = "Water reaches its goal by flowing continually. It fills up every depression\nbefore it flows on. The superior man follows its example; he is concerned\nthat goodness should be an established attribute of character rather\nthan an accidental and isolated occurrence. So likewise in teaching others\neverything depends on consistency, for it is only through repetition that\nthe pupil makes the material his own.",
        .lines = {
        {
          .pd ="Repetition of the Abysmal.\nIn the abyss one falls into a pit.\nMisfortune.",
          .cm = "By growing used to what is dangerous, a man can easily allow it to\nbecome part of him. He is familiar with it and grows used to evil. With\nthis he has lost the right way, and misfortune is the natural result.",
        },
        {
          .pd ="The abyss is dangerous.\nOne should strive to attain small things only.",
          .cm = "When we are in danger we ought not to attempt to get out of it immediately,\nregardless of circumstances; at first we must content ourselves with\nnot being overcome by it. We must calmly weigh the conditions of the time\nand by satisfied with small gains, because for the time being a great\nsuccess cannot be attained. A spring flows only sparingly at first, and\ntarries for some time before it makes its way in to the open.",
        },
        {
          .pd ="Forward and backward, abyss on abyss.\nIn danger like this, pause at first and wait,\nOtherwise you will fall into a pit in the abyss.\nDo not act this way.",
          .cm = "Here every step, forward or backward, leads into danger. Escape is\nout of the question. Therefore we must not be misled into action, as\na result of which we should only bog down deeper in the danger; disagreeable\nas it may be to remain in such a situation, we must wait until a way out\nshows itself.",
        },
        {
          .pd ="A jug of wine, a bowl of rice with it;\nEarthen vessels\nSimply handed in through the Window.\nThere is certainly no blame in this.",
          .cm = "In times of danger ceremonious forms are dropped. What matters most\nis sincerity. Although as a rule it is customary for an official to present\ncertain introductory gifts and recommendations before he is appointed,\nhere everything is simplified to the utmost. The gifts are insignificant,\nthere is no one to sponsor him, he introduces himself; yet all this need\nnot be humiliating if only there is the honest intention of mutual help\nin danger. Still another idea is suggested. The window is the place through\nwhich light enters the room. If in difficult times we want to enlighten\nsomeone, we must begin with that which is in itself lucid and proceed\nquite simply from that point on.",
        },
        {
          .pd ="The abyss is not filled to overflowing,\nIt is filled only to the rim.\nNo blame.",
          .cm = "Danger comes because one is too ambitious. In order to flow out of\na ravine, water does not rise higher than the lowest point of the rim.\nSo likewise a man when in danger has only to proceed along the line of\nleast resistance; thus he reaches the goal. Great labors cannot be accomplished\nin such times; it is enough to get out of the danger.",
        },
        {
          .pd ="Bound with cords and ropes,\nShut in between thorn-hedged prison walls:\nFor three years one does not find the way.\nMisfortune.",
          .cm = "A man who in the extremity of danger has lost the right way and is\nirremediably entangled in his sins has no prospect of escape. He is like\na criminal who sits shackled behind thorn hedged prison walls.",
        }
      }
  },
  {
    .number = 30,
    .sk = 0b00101101,
    .name = "The Clinging, Fire",
    .cm = "This hexagram is another double sign. The trigram Li means \"to cling\nto something,\" and also \"brightness.\" A dark line clings to two light\nlines, one above and one below--the image of an empty space between two\nstrong lines, whereby the two strong lines are made bright. The trigram\nrepresents the middle daughter. The Creative has incorporated the central\nline of the Receptive, and thus Li develops. As an image, it is fire. Fire\nhas no definite form but clings to the burning object and thus is bright.\nAs water pours down from heaven, so fire flames up from the earth. While\nK'an means the soul shut within the body, Li stands for nature in its radiance.",
    .jd = "THE CLINGING. Perseverance furthers.\nIt brings success.\nCare of the cow brings good fortune.",
    .j_cm = "What is dark clings to what is light and so enhances the brightness\nof the latter. A luminous thing giving out light must have within itself\nsomething that perseveres; otherwise it will in time burn itself out.\nEverything that gives light is dependent on something to which it clings,\nin order that it may continue to shine. Thus the sun and moon cling to\nheaven, and grain, grass, and trees cling to the earth. So too the twofold\nclarity of the dedicated man clings to what is right and thereby can shape\nthe world. Human life on earth is conditioned and unfree,\nand when man recognizes this limitation\nand makes himself dependent upon the harmonious and beneficent\nforces of the cosmos, he achieves success. The cow is the symbol of extreme\ndocility. By cultivating in himself an attitude of compliance and voluntary\ndependence, man acquires clarity without sharpness and finds his place\nin the world.",
    .im =   "That which is bright rises twice:\nThe image of FIRE.\nThus the great man, by perpetuating this brightness,\nIllumines the four quarters of the world.",
    .i_cm = "Each of the two trigrams represents the sun in the course of a day.\nThe two together represent the repeated movement of the sun, the function\nof light with respect to time. The great man continues the work of nature\nin the human world. Through the clarity of his nature he causes the light\nto spread farther and farther and to penetrate the nature of man ever\nmore deeply.",
        .lines = {
        {
          .pd ="The footprints run crisscross.\nIf one is seriously intent, no blame.",
          .cm = "It is early morning and work begins. The mind has been closed to the\noutside world in sleep; now its connections with the world begin again.\nThe traces of one's impressions run crisscross. Activity and haste prevail.\nIt is important then to preserve inner composure and not to allow oneself\nto be swept along by the bustle of life. If one is serious and composed,\nhe can acquire the clarity of mind needed for coming to terms with the\ninnumerable impressions that pour in. It is precisely at the beginning\nthat serious concentration is important, because the beginning holds the\nseed of all that is to follow.",
        },
        {
          .pd ="Yellow light. Supreme good fortune.",
          .cm = "Midday has come; the sun shines with a yellow light. Yellow is the\ncolor of measure and mean. Yellow light is therefore a symbol of the\nhighest culture and art, whose consummate harmony consists in holding\nto the mean.",
        },
        {
          .pd ="In the light of the setting sun,\nMen either beat the pot and sing\nOr loudly bewail the approach of old age.\nMisfortune.",
          .cm = "Here the end of the day has come. The light of the setting sun calls\nto mind the fact that life is transitory and conditional. Caught in this\nexternal bondage, men are usually robbed of their inner freedom as well.\nThe sense of the transitoriness of life impels them to uninhibited revelry\nin order to enjoy life while it lasts, or else they yield to melancholy\nand spoil the precious time by lamenting the approach of old age. Both attitudes\nare wrong. To the superior man it makes no difference whether death comes\nearly or late. He cultivates himself, awaits his allotted time, and in\nthis way secures his fate.",
        },
        {
          .pd ="Its coming is sudden;\nIt flames up, dies down, is thrown away.",
          .cm = "Clarity of mind has the same relation to life that fire has to wood.\nFire clings to wood, but also consumes it. Clarity of mind is rooted\nin life but can also consume it. Everything depends upon how the clarity\nfunctions. Here the image used is that of a meteor or a straw fire. A\nman who is excitable and restless may rise quickly to prominence but produces\nno lasting effects. Thus matters end badly when a man spends himself too\nrapidly and consumes himself like a meteor.",
        },
        {
          .pd ="Tears in floods, sighing and lamenting.\nGood fortune.",
          .cm = "Here the zenith of life has been reached. Were there no warning, one\nwould at this point consume oneself like a flame. Instead, understanding\nthe vanity of all things, one may put aside both hope and fear, and sigh\nand lament: if one is intent on retaining his clarity of mind, good fortune\nwill come from this grief. For here we are dealing not with a passing mood,\nas in the nine in the third place, but with a real change of heart.",
        },
        {
          .pd ="The king used him to march forth and chastise.\nThen it is best to kill the leaders\nAnd take captive the followers. \nNo blame.",
          .cm = "It is not the purpose of chastisement to impose punishment blindly\nbut to create discipline. Evil must be cured at its roots. To eradicate\nevil in political life, it is best to kill the ringleaders and spare the\nfollowers. In educating oneself it is best to root out bad habits and\ntolerate those that are harmless. For asceticism that is too strict, like\nsentences of undue severity, fails in its purpose.",
        }
      }
  },
  {
    .number = 31,
    .sk = 0b00011100,
    .name = "Influence (Wooing)",
    .cm = "The name of the hexagram means \"universal,\" \"general,\" and in a figurative\nsense \"to influence,\" \"to stimulate.\" The upper trigram is Tui, the Joyous;\nthe lower is Kên, Keeping still. By its persistent, quiet influence,\nthe lower, rigid trigram stimulates the upper, weak trigram, which responds\nto this stimulation cheerfully and joyously. Kên, the lower trigram,\nis the youngest son; the upper, Tui, is the youngest daughter. Thus the\nuniversal mutual attraction between the sexes is represented. In courtship,\nthe masculine principle must seize the initiative and place itself below\nthe feminine principle. Just as the first part of book 1 begins with\nthe hexagrams of heaven and earth, the foundations of all that exists,\nthe second part begins with the hexagrams of courtship and marriage, the\nfoundations of all social relationships.",
    .jd = "Influence. Success.\nPerseverance furthers.\nTo take a maiden to wife brings good fortune.",
    .j_cm = "The weak element is above, the strong below; hence their powers attract\neach other, so that they unite. This brings about success, for all success\ndepends on the effect of mutual attraction. By keeping still within while\nexperiencing joy without, one can prevent the joy from going to excess\nand hold it within proper bounds. This is the meaning of the added admonition,\n\"Perseverance furthers,\" for it is perseverance that makes the difference\nbetween seduction and courtship; in the latter the strong man takes a\nposition inferior to that of the weak girl and shows consideration for\nher. This attraction between affinities is a general law of nature. Heaven\nand earth attract each other and thus all creatures come into being. Through\nsuch attraction the sage influences men's hearts, and thus the world attains\npeace. From the attractions they exert we can learn the nature of all beings\nin heaven and on earth.",
    .im =   "A lake on the mountain:\nThe image of influence.\nThus the superior man encourages people to approach him\nBy his readiness to receive them.",
    .i_cm = "A mountain with a lake on its summit is stimulated by the moisture\nfrom the lake. It has this advantage because its summit does not jut\nout as a peak but is sunken. The image counsels that the mind should be\nkept humble and free, so that it may remain receptive to good advice. People\nsoon give up counseling a man who thinks that he knows everything better\nthan anyone else.",
        .lines = {
        {
          .pd ="The influence shows itself in the big toe.",
          .cm = "A movement, before it is actually carried out, shows itself first\nin the toes. The idea of an influence is already present, but is not immediately\napparent to others. As long as the intention has no visible effect, it\nis of no importance to the outside world and leads neither to good nor\nto evil.",
        },
        {
          .pd ="The influence shows itself in the calves of the legs.\nMisfortune.\nTarrying brings good fortune.",
          .cm = "In movement, the calf of the leg follows the foot; by itself it can\nneither go forward nor stand still. Since the movement is not self-governed,\nit bodes ill. One should wait quietly until one is impelled to action\nby a real influence. Then one remains uninjured.",
        },
        {
          .pd ="The influence shows itself in the thighs.\nHolds to that which follows it.\nTo continue is humiliating.",
          .cm = "Every mood of the heart influences us to movement. What the heart\ndesires, the thighs run after without a moment's hesitation; they hold to\nthe heart, which they follow. In the life of man, however, acting on the\nspur of every caprice is wrong and if continued leads to humiliation. Three\nconsiderations suggest themselves here. First, a man should not run precipitately\nafter all the persons whom he would like to influence, but must be able to\nhold back under certain circumstances. As little should he yield immediately\nto every whim of those in whose service he stands. Finally, where the moods\nof his own heart are concerned, he should never ignore the possibility of\ninhibition, for this is the basis of human freedom.",
        },
        {
          .pd ="Perseverance brings good fortune.\nRemorse disappears.\nIf a man is agitated in mind,\nAnd his thoughts go hither and thither,\nOnly those friends\nOn whom he fixes his conscious thoughts\nWill follow.",
          .cm = "Here the place of the heart is reached. The impulse that springs from\nthis source is the most important of all. It is of particular concern\nthat this influence be constant and good; then, in spite of the danger\narising from the great susceptibility of the human heart, there will be\nno cause for remorse. When the quiet power of a man's own character is\nat work, the effects produced are right. All those who are receptive to\nthe vibrations of such a spirit will then be influenced. Influence over\nothers should not express itself as a conscious and willed effort to manipulate\nthem. Through practicing such conscious incitement, one becomes wrought\nup and is exhausted by the eternal stress and strain. Moreover, the effects\nproduced are then limited to those on whom one's thoughts are consciously\nfixed.",
        },
        {
          .pd ="The influence shows itself in the back of the neck.\nNo remorse.",
          .cm = "The back of the neck is the most rigid part of the body. When the\ninfluence shows itself there, the will remains firm and the influence does\nnot lead to confusion. Hence remorse does not enter into consideration here.\nWhat takes place in the depths of one's being, in the unconscious mind. It\nis true that if we cannot be influenced ourselves, we cannot influence the\noutside world.",
        },
        {
          .pd ="The influence shows itself in the jaws, cheeks, and tongue.",
          .cm = "The most superficial way of trying to influence others is through\ntalk that has nothing real behind it. The influence produced by such mere\ntongue wagging must necessarily remain insignificant. Hence no indication\nis added regarding good or bad fortune.",
        }
      }
  },
  {
    .number = 32,
    .sk = 0b00001110,
    .name = "Duration",
    .cm = "The strong trigram Chên is above, the weak trigram Sun below.\nThis hexagram is the inverse of the preceding one. In the latter we have\ninfluence, here we have union as an enduring condition. The two images\nare thunder and wind, which are likewise constantly paired phenomena. The\nlower trigram indicates gentleness within; the upper, movement without.\nIn the sphere of social relationships, the hexagram represents the institution\nof marriage as the enduring union of the sexes. During courtship the young\nman subordinates himself to the girl, but in marriage, which is represented\nby the coming together of the eldest son and the eldest daughter, the husband\nis the directing and moving force outside, while the wife, inside, is\ngentle and submissive.",
    .jd = "DURATION. Success. No blame.\nPerseverance furthers.\nIt furthers one to have somewhere to go.",
    .j_cm = "Duration is a state whose movement is not worn down by hindrances.\nIt is not a state of rest, for mere standstill is regression. Duration\nis rather the self-contained and therefore self-renewing movement of an\norganized, firmly integrated whole, taking place in accordance with immutable\nlaws and beginning anew at every ending. The end is reached by an inward\nmovement, by inhalation, systole, contraction, and this movement turns\ninto a new beginning, in which the movement is directed outward, in exhalation,\ndiastole, expansion. Heavenly bodies exemplify duration. They move in\ntheir fixed orbits, and because of this their light-giving power endures.\nThe seasons of the year follow a fixed law of change and transformation,\nhence can produce effects that endure. So likewise the dedicated man embodies\nan enduring meaning in his way of life, and thereby the world is formed.\nIn that which gives things their duration, we can come to understand the\nnature of all beings in heaven and on earth.",
    .im =   "Thunder and wind: the image of DURATION.\nThus the superior man stands firm\nAnd does not change has direction.",
    .i_cm = "Thunder rolls, and the wind blows; both are examples of extreme mobility\nand so are seemingly the very opposite of duration, but the laws governing\ntheir appearance and subsidence, their coming and going, endure. In the\nsame way the independence of the superior man is not based on rigidity\nand immobility of character. He always keeps abreast of the time and changes\nwith it. What endures is the unswerving directive, the inner law of his\nbeing, which determines all his actions.",
        .lines = {
        {
          .pd ="Seeking duration too hastily brings misfortune persistently.\nNothing that would further.",
          .cm = "Whatever endures can be created only gradually by long-continued work\nand careful reflection. In the same sense Lao-tse says: \"If we wish to\ncompress something, we must first let it fully expand.\" He who demands\ntoo much at once is acting precipitately, and because he attempts too much,\nhe ends by succeeding in nothing.",
        },
        {
          .pd ="Remorse disappears.",
          .cm = "The situation is abnormal. A man's force of character is greater\nthan the available material power. Thus he might be afraid of allowing\nhimself to attempt something beyond his strength. However, since it is\nthe time of DURATION, it is possible for him to control his inner strength\nand so to avoid excess. Cause for remorse then disappears.",
        },
        {
          .pd ="He who does not give duration to his character\nMeets with disgrace.\nPersistent humiliation.",
          .cm = "If a man remains at the mercy of moods of hope or fear aroused by\nthe outer world, he loses his inner consistency of character. Such inconsistency\ninvariably leads to distressing experiences. These humiliations often\ncome from an unforeseen quarter. Such experiences are not merely effects\nproduced by the external world, but logical consequences evoked by his\nown nature.",
        },
        {
          .pd ="No game in the field.",
          .cm = "If we are in pursuit of game and want to get a shot at a quarry, we\nmust set about it in the right way. A man who persists in stalking game\nin a place where there is none may wait forever without finding any. Persistence\nin search is not enough. What is not sought in the right way is not found.",
        },
        {
          .pd ="Giving duration to one's character through perseverance.\nThis is good fortune for a woman, misfortune for a man.",
          .cm = "A woman should follow a man her whole life long, but a man should\nat all times hold to what is his duty at the given moment.\nShould he persistently seek to conform to the woman,\nit would be a mistake for him. Accordingly it is altogether right\nfor a woman to hold conservatively to tradition,\nbut a man must always be flexible and adaptable and allow himself to be\nguided solely by what his duty requires of him at the moment.",
        },
        {
          .pd ="Restlessness as an enduring condition brings misfortune.",
          .cm = "There are people who live in a state of perpetual hurry without ever\nattaininginner composure. Restlessness not only prevents all thoroughness\nbut actuallybecomes a danger if it is dominant in places of authority.",
        }
      }
  },
  {
    .number = 33,
    .sk = 0b00111100,
    .name = "Retreat",
    .cm = "The power of the dark is ascending. The light retreats to security,\nso that the dark cannot encroach upon it. This retreat is a matter not\nof man's will but of natural law. Therefore in this case withdrawal is\nproper; it is the correct way to behave in order not to exhaust one's forces.\nIn the calendar this hexagram is linked with the sixth month (July-August),\nin which the forces of winter are already showing their influence.",
    .jd = "RETREAT. Success.\nIn what is small, perseverance furthers.",
    .j_cm = "Conditions are such that the hostile forces favored by the time are\nadvancing. In this case retreat is the right course, and it is not to\nbe confused with flight. Flight means saving oneself under any circumstances,\nwhereas retreat is a sign of strength. We must be careful not to miss\nthe right moment while we are in full possession of power and position.\nThen we shall be able to interpret the signs of the time before it is too\nlate and to prepare for provisional retreat instead of being drawn into\na desperate life-and-death struggle. Thus we do not simple abandon the\nfield to the opponent; we make it difficult for him to advance by showing\nperseverance in single acts of resistance. In this way we prepare, while\nretreating, for the counter-movement. Understanding the laws of a constructive\nretreat of this sort is not easy. The meaning that lies hidden in such\na time is important.",
    .im =   "Mountain under heaven: \nthe image of RETREAT.\nThus the superior man keeps the inferior man at a distance,\nNot angrily but with reserve.",
    .i_cm = "The mountain rises up under heaven, but owing to its nature it finally\ncomes to a stop. Heaven on the other hand retreats upward before it into\nthe distance and remains out of reach. This symbolizes the behavior of\nthe superior man toward a climbing inferior; he retreats into his own thoughts\nas the inferior man comes forward. He does not hate him, for hatred is\na form of subjective involvement by which we are bound to the hated object.\nThe superior man shows strength (heaven) in that he brings the inferior\nman to a standstill (mountain) by his dignified reserve.",
        .lines = {
        {
          .pd ="At the tail in retreat. This is dangerous.\nOne must not wish to undertake anything.",
          .cm = "Since the hexagram is the picture of something that is retreating,\nthe lowest line represents the tail and the top line the head. In a retreat\nit is advantageous to be at the front. Here one is at the back, in immediate\ncontact with the pursuing enemy. This is dangerous, and under such circumstances\nit is not advisable to undertake anything. Keeping still is the easiest\nway of escaping from the threatening danger.",
        },
        {
          .pd ="he holds him fast with yellow oxhide.\nNo one can tear him loose.",
          .cm = "Yellow is the color of the middle. It indicates that which is correct\nand in line with duty. Oxhide is strong and not to be torn. While the\nsuperior men retreat and the inferior press after them, the inferior man\nrepresented here holds on so firmly and tightly to the superior man that\nthe latter cannot shake him off. And because he is in quest of what is\nright an so strong in purpose, he reaches his goal. Thus the line confirms\nwhat is said in the Judgment: \"In what is small\" --here equivalent to \"in\nthe inferior man\" -- \"perseverance furthers.\"",
        },
        {
          .pd ="A halted retreat\nIs nerve-wracking and dangerous.\nTo retain people as men- and maidservants\nBrings good fortune.",
          .cm = "When it is time to retreat it is both unpleasant and dangerous to\nbe held back, because then one no longer has freedom of action. In such a\ncase the only expedient is to take into one's service, so to speak, those\nwho refuse to let one go, so that one may at least keep one's initiative\nand not fall helplessly under their domination. But even with this expedient\nthe situation is far from satisfactory--for what can one hope to accomplish\nwith such servants?",
        },
        {
          .pd ="Voluntary retreat brings good fortune to the superior man\nAnd downfall to the inferior man.",
          .cm = "In retreating the superior man is intent on taking his departure willingly\nand in all friendliness. He easily adjusts his mind to retreat, because\nin retreating he does not have to do violence to his convictions. The\nonly one who suffers is the inferior man from whom he retreats, who will\ndegenerate when deprived of the guidance of the superior man.",
        },
        {
          .pd ="Friendly retreat. Perseverance brings good fortune.",
          .cm = "It is the business of the superior man to recognize in time that the\nmoment for retreat has come. If the right moment is chosen, the retreat\ncan be carried out within the forms of perfect friendliness, without the\nnecessity of disagreeable discussions. Yet, for all the observance of\namenities, absolute firmness of decision is necessary if one is not to\nbe led astray by irrelevant considerations.",
        },
        {
          .pd ="Cheerful retreat. Everything serves to further.",
          .cm = "The situation is unequivocal. Inner detachment has become an established\nfact, and we are at liberty to depart. When one sees the way ahead thus\nclearly, free of all doubt, a cheerful mood sets in, and one chooses what\nis right without further thought. Such a clear path ahead always leads\nto the good.",
        }
      }
  },
  {
    .number = 34,
    .sk = 0b00001111,
    .name = "The Power of the Great",
    .cm = "The great lines, that is, the light, strong lines, are powerful. Four\nlight lines have entered the hexagram from below and are about to ascend\nhigher. The upper trigram is Chên, the Arousing; the lower is ch'ien,\nthe Creative. Ch'ien is strong, Chên produces movement. The union\nof movement and strength gives the meaning of THE POWER OF THE GREAT. The\nhexagram is linked with the second month (March-April).",
    .jd = "THE POWER OF THE GREAT. Perseverance furthers.",
    .j_cm = "The hexagram points to a time when inner worth mounts with great force\nand comes to power. But its strength has already passed beyond the median\nline, hence there is danger that one may rely entirely on one's own power\nand forget to ask what is right. There is danger too that, being intent\non movement, we may not wait for the right time. Therefore the added statement\nthat perseverance furthers. For that is truly great power which does\nnot degenerate into mere force but remains inwardly united with the fundamental\nprinciples of right and of justice. When we understand this point--namely,\nthat greatness and justice must be indissolubly united--we understand\nthe true meaning of all that happens in heaven and on earth.",
    .im =   "Thunder in heaven above:\nThe image of THE POWER OF THE GREAT.\nThus the superior man does not tread upon paths\nThat do not accord with established order.",
    .i_cm = "Thunder--electrical energy--mounts upward in the spring. The direction\nof this movement is in harmony with that of the movement of heaven. It\nis therefore a movement in accord with heaven, producing great power.\nHowever, true greatness depends on being in harmony with what is right.\nTherefore in times of great power the superior man avoids doing anything\nthat is not in harmony with the established order.",
        .lines = {
        {
          .pd ="Power in the toes.\nContinuing brings misfortune.\nThis is certainly true.",
          .cm = "The toes are in the lowest place and are ready to advance. So likewise\ngreat power in lowly station is inclined to effect advance by force.\nThis, if carried further, would certainly lead to misfortune, and therefore\nby way of advice a warning is added.",
        },
        {
          .pd ="Perseverance brings good fortune.",
          .cm = "The premise here is that the gates to success are beginning to open.\nResistance gives way and we forge ahead. This is the point at which,\nonly too easily, we become the prey of exuberant self-confidence. This\nis why the oracle says that perseverance (i.e., perseverance in inner\nequilibrium, without excessive use of power) brings good fortune.",
        },
        {
          .pd ="The inferior man works through power.\nThe superior man does not act thus.\nTo continue is dangerous.\nA goat butts against a hedge\nAnd gets its horns entangled.",
          .cm = "Making a boast of power leads to entanglements, just as a goat entangles\nits horns when it butts against a hedge. Whereas an inferior man revels\nin power when he comes into possession of it, the superior man never makes\nthis mistake. He is conscious at all times of the danger of pushing ahead\nregardless of circumstances, and therefore renounces in good time the\nempty display of force.",
        },
        {
          .pd ="Perseverance brings good fortune.\nRemorse disappears.\nThe hedge opens; there is no entanglement.\nPower depends upon the axle of a big cart.",
          .cm = "If a man goes on quietly and perseveringly working at the removal\nof resistances, success comes in the end. The obstructions give way and all\noccasion for remorse arising from excessive use of power disappears.\nSuch a man's power does not show externally, yet it can move heavy\nloads, like a big cart whose real strength lies in its axle. The less\nthat power is applied outwardly, the greater its effect.",
        },
        {
          .pd ="Loses the goat with ease.\nNo remorse.",
          .cm = "The goat is noted for hardness outwardly and weakness within. Now\nthe situation is such that everything is easy; there is no more resistance.\nOne can give up a belligerent, stubborn way of acting and will not have to\nregret it.",
        },
        {
          .pd ="A goat butts against a hedge.\nIt cannot go backward, it cannot go forward.\nNothing serves to further.\nIf one notes the difficulty, this brings good fortune.",
          .cm = "If we venture too far we come to a deadlock, unable either to advance\nor to retreat and whatever we do merely serves to complicate thing further.\nSuch obstinacy leads to insuperable difficulties. But if, realizing the\nsituation, we compose ourselves and decide not to continue, everything\nwill right itself in time.",
        }
      }
  },
  {
    .number = 35,
    .sk = 0b00101000,
    .name = "Progress",
    .cm = "The hexagram represents the sun rising over the earth. It is therefore\nthe symbol of rapid, easy progress, which at the same time means ever\nwidening expansion and clarity.",
    .jd = "PROGRESS. \nThe powerful prince\nIs honored with horses in large numbers.\nIn a single day he is granted audience three times.",
    .j_cm = "As an example of progress, this pictures a time when a powerful feudal\nlord rallies the other lords around the sovereign and pledges fealty\nand peace. The sovereign rewards him richly and\ninvites him to a closer intimacy.\n\nA twofold idea is set forth here. The actual effect of the progress\nemanates from a man who is in a dependent position and whom the others\nregard as their equal and are therefore willing to follow. This leader\nhas enough clarity of vision not to abuse his great influence but to use\nit rather for the benefit of his ruler. His ruler in turn is free of all\njealousy, showers presents on the great man, and invites him continually\nto his court. An enlightened ruler and an obedient servant--this is the\ncondition on which great progress depends.",
    .im =   "The sun rises over the earth:\nThe image of PROGRESS.\nThus the superior man himself\nBrightens his bright virtue.",
    .i_cm = "The light of the sun rises over the earth is by nature clear. The\nhigher the sun rises, the more it emerges from the dark mists, spreading\nthe pristine purity of its rays over an ever widening area. The real nature\nof man is likewise originally good, but it becomes clouded by contact with\nearthly things and therefore needs purification before it can shine forth\nin its native clarity.",
        .lines = {
        {
          .pd ="Progressing, but turned back.\nPerseverance brings good fortune.\nIf one meets with no confidence, one should remain calm.\nNo mistake.",
          .cm = "At a time when all elements are pressing for progress, we are still\nuncertain whether in the course of advance we may not meet with a rebuff.\nThen the thing to do is simply continue in what is right; in the end\nthis will bring good fortune. It may be that we meet with no confidence.\nIn this case we ought not to try to win confidence regardless of the situation,\nbut should remain calm and cheerful and refuse to be roused to anger.\nThus we remain free of mistakes.",
        },
        {
          .pd ="Progressing, but in sorrow.\nPerseverance brings good fortune.\nThen one obtains great happiness from one's ancestress.",
          .cm = "Progress is halted; an individual is kept from getting in touch with\nthe man in authority with whom he has a connection. When this happens,\nhe must remain persevering, although he is grieved; then with a maternal\ngentleness the man in question will bestow great happiness upon him. This\nhappiness comes to him-and is well deserved-because in this case mutual\nattraction does not rest on selfish or partisan motives but on firm and correct\nprinciples.",
        },
        {
          .pd ="All are in accord. Remorse disappears.",
          .cm = "A man strives onward, in association with others whose backing encourages\nhim. This dispels any cause for regret over the fact that he does not\nhave enough independence to triumph unaided over every hostile turn of\nfate.",
        },
        {
          .pd ="Progress like a hamster.\nPerseverance brings danger.",
          .cm = "In times of progress it is easy for strong men in the wrong places\nto amass great possessions. But such conduct shuns the light. And since\ntimes of progress are inevitably brought to the light, perseverance in\nsuch action always leads to danger.",
        },
        {
          .pd ="Remorse disappears.\nTake not gain and loss to heart.\nUndertakings bring good fortune.\nEverything serves to further.",
          .cm = "The situation described here is that of one who, finding himself in\nan influential position in a time of progress, remains gentle and reserved.\nHe might reproach himself for lack of energy in making the most of the\npropitiousness of the time and obtaining all possible advantage. However,\nthis regret passes away. He must not take either loss or gain to heart;\nthey are minor considerations. What matters much more is the fact that\nin this way he has assured himself of opportunities for successful and\nbeneficent influence.",
        },
        {
          .pd ="Making progress with the horns is permissible\nOnly for the purpose of punishing one's own city.\nTo be conscious of danger brings good fortune.\nNo blame.\nPerseverance brings humiliation.",
          .cm = "Making progress with lowered horns-i.e., acting on the offensive-is\npermissible, in times like those referred to here, only in dealing with\nthe mistakes of one's own people. Even then we must bear in mind that\nproceeding on the offensive may always be dangerous. In this way we avoid\nthe mistakes that otherwise threaten, and succeed in what we set out\nto do. On the other hand, perseverance in such over energetic behavior,\nespecially toward persons with whom there is no close connection, will\nlead to humiliation.",
        }
      }
  },
  {
    .number = 36,
    .sk = 0b00000101,
    .name = "Darkening of the light",
    .cm = "Here the sun has sunk under the earth and is therefore darkened. The\nname of the hexagram means literally \"wounding of the bright\"; hence\nthe individual lines contain frequent references to wounding. The situation\nis the exact opposite of that in the foregoing hexagram. In the latter\na wise man at the head of affairs has able helpers, and in company with\nthem makes progress; here a man of dark nature is in a position of authority\nand brings harm to the wise and able man.",
    .jd = "DARKENING OF THE LIGHT. In adversity\nIt furthers one to be persevering.",
    .j_cm = "One must not unresistingly let himself be swept along by unfavorable\ncircumstances, nor permit his steadfastness to be shaken. He can avoid\nthis by maintaining his inner light, while remaining outwardly yielding\nand tractable. With this attitude he can overcome even the greatest adversities.\nIn some situations indeed a man must hide his light, in order to make\nhis will prevail inspite of difficulties in his immediate environment.\nPerseverance must dwell in inmost consciousness and should not be discernible\nfrom without. Only thus is a man able to maintain his will in the face of\ndifficulties.",
    .im =   "The light has sunk into the earth:\nThe image of DARKENING OF THE LIGHT.\nThus does the superior man live with the great mass:\nHe veils his light, yet still shines.",
    .i_cm = "In a time of darkness it is essential to be cautious and reserved.\nOne should not needlessly awaken overwhelming enmity by inconsiderate\nbehavior. In such times one ought not to fall in with the practices of\nothers; neither should one drag them censoriously into the light. In\nsocial intercourse one should not try to be all-knowing. One should let\nmany things pass, without being duped.",
        .lines = {
        {
          .pd ="Darkening of the light during flight.\nHe lowers his wings.\nThe superior man does not eat for three days\nOn his wanderings.\nBut he has somewhere to go.\nThe host has occasion to gossip about him.",
          .cm = "With grandiose resolve a man endeavors to soar above all obstacles,\nbut thus encounters a hostile fate. He retreats and evades the issue.\nThe time is difficult. Without rest, he must hurry along, with no permanent\nabiding place. If he does not want to make compromises within himself,\nbut insists on remaining true to his principles, he suffers deprivation.\nNever the less he has a fixed goal to strive for even though the people\nwith whom he lives do not understand him and speak ill of him.",
        },
        {
          .pd ="Darkening of the light injures him in the left thigh.\nHe gives aid with the strength of a horse.\nGood fortune.",
          .cm = "Here the Lord of Light is in a subordinate place and is wounded by\nthe Lord of Darkness. But the injury is not fatal; it is only a hindrance.\nRescue is still possible. The wounded man gives no thought to himself;\nhe thinks only of saving the others who are also in danger. Therefore\nhe tries with all his strength to save all that can be saved. There is\ngood fortune in thus acting according to duty.",
        },
        {
          .pd ="Darkening of the light during the hunt in the south.\nTheir great leader is captured.\nOne must not expect perseverance too soon.",
          .cm = "It seems as if chance were at work. While the strong, loyal man is\nstriving eagerly and in good faith to create order, he meets the ringleader\nof the disorder, as if by accident, and seizes him. Thus victory is achieved.\nBut in abolishing abuses one must not be too hasty. This would turn\nout badly because the abuses have been in existence so long.",
        },
        {
          .pd ="He penetrates the left side of the belly.\nOne gets at the very heart of the darkening of the light.",
          .cm = "We find ourselves close to the commander of darkness and so discover\nhis mot secret thoughts. In this way we realize that there is no longer\nany hope of improvement, and thus we are enabled to leave the scene of\ndisaster before the storm breaks.",
        },
        {
          .pd ="Darkening of the light as with Prince Chi.\nPerseverance furthers.",
          .cm = "Prince Chi lived at the court of the evil tyrant Chou Hsin, who, although\nnot mentioned by name, furnished the historical example on which this\nwhole situation is based. Prince Chi was a relative of the tyrant and\ncould not withdraw from the court; therefore he concealed his true sentiments\nand feigned insanity. Although he was held a slave, he did not allow external\nmisery to deflect him from his convictions. This provides a teaching for\nthose who cannot leave their posts in times of darkness. In order to escape\ndanger, they need invincible perseverance of spirit and redoubled caution\nin their dealings with the world.",
        },
        {
          .pd ="Not light but darkness.\nFirst he climbed up to heaven,\nThen plunged into the depths of the earth.",
          .cm = "Here the climax of the darkening is reached. The dark power at first\nheld so high a place that it could wound all who were on the side of\ngood and of the light. But in the end it perishes of its own darkness,\nfor evil must itself fall at the very moment when it has wholly overcome\nthe good, and thus consumed the energy to which it owed its duration.",
        }
      }
  },
  {
    .number = 37,
    .sk = 0b00110101,
    .name = "The Family [The Clan]",
    .cm = "The hexagram represents the laws obtaining within the family. The\nstrong line at the top represents the father, the lowest the son. The strong\nline in the fifth place represents the husband, the yielding second line\nthe wife. On the other hand, the two strong lines in the fifth and the third\nplace represent two brothers, and the two weak lines correlated with them\nin the fourth and the second place stand for their respective wives. Thus\nall the connections and relationships within the family find their appropriate\nexpression. Each individual line has the character according with its place.\nThe fact that a strong line occupies the sixth place-where a weak line might\nbe expected-indicates very clearly the strong leadership that must come from\nthe head of the family. The line is to be considered here not in its quality\nas the sixth but in its quality as the top line. THE FAMILY shows the laws\noperative within the household that, transferred to outside life, keep the\nstate and the world in order. The influence that goes out from within the\nfamily is represented by the symbol of the wind created by fire.",
    .jd = "THE FAMILY. The perseverance of the woman furthers.",
    .j_cm = "The foundation of the family is the relationship between husband and\nwife. The tie that hold the family together lies in the loyalty and perseverance\nof the wife. The tie that holds the family together lies in the loyalty\nand perseverance of the wife. Her place is within (second line), while\nthat of the husband is without (fifth line). It is in accord with the great\nlaws of nature that husband and wife take their proper places. Within the\nfamily a strong authority is needed; this is represented by the parents.\nIf the father is really a father and the son a son, if the elder brother\nfulfills his position, and the younger fulfills his, if the husband is really\na husband and the wife a wife, then the family is in order. When the family\nis in order, all the social relationships of mankind will be in order.\nThree of the five social relationships are to be found within the family-that\nbetween father and son, which is the relation of love, that between the\nhusband and wife, which is the relation of chaste conduct, and that between\nelder and younger brother, which is the relation of correctness.\nThe loving reverence of the son is then carried over to the prince\nin the form of faithfulness to duty;\nthe affection and correctness of behavior existing between the\ntwo brothers are extended to a friend in the form of loyalty, and to a person\nof superior rank in the form of deference. The family is society in the\nembryo; it is the native soil on which performance of moral duty is made\nearly through natural affection, so that within a small circle a basis of\nmoral practice is created,\nand this is later widened to include human relationships\nin general.",
    .im =   "Wind comes forth from fire:\nThe image of THE FAMILY.Thus the superior man has substance in his\nwords\nAnd duration in his way of life.",
    .i_cm = "Heat creates energy: this is signified by the wind stirred up by the\nfire and issuing forth form it. This represents influence working from\nwithin outward. The same thing is needed in the regulation of the family.\nHere too the influence on others must proceed form one's own person. In\norder to be capable of producing such an influence, one's words must have\npower, and this they can have only if they are based on something real, just\nas flame depends on its fuel Words have influence only when they are pertinent\nand clearly related to definite circumstances. General discourses and admonitions\nhave no effect whatsoever. Furthermore, the words must be supported by\none's entire conduct, just as the wind is made effective by am impression\non others that they can adapt and conform to it. If words and conduct are\nnot in accord and consistent, they will have no effect.",
        .lines = {
        {
          .pd ="Firm seclusion within the family.\nRemorse disappears.",
          .cm = "The family must form a well-defined unit within which each member\nknows his place. From the beginning each child must be accustomed to firmly\nestablished rules of order, before ever its will is directed to other things.\nIf we begin too late to enforce order, when the will of the child has already\nbeen overindulged, the whims and passions, grown stronger with the years,\noffer resistance and give cause for remorse. If we insist on order from the\noutset, occasions for remorse may arise-in general social life these are\nunavoidable-but the remorse always disappears again, and everything rights\nitself. For there is nothing easily avoided and more difficult to carry through\nthan \"breaking a child's will.\"",
        },
        {
          .pd ="She should not follow her whims.\nShe must attend within to the food.\nPerseverance brings good fortune.",
          .cm = "The wife must always be guided by the will of the master of the house,\nbe he father, husband, or grown son. There, without having to look for\nthem, she has great and important duties. She must attend to the nourishment\nof her family and to the food for the sacrifice. IN this way she becomes\nthe center of the social and religious life of the family, and her perseverance\nin this position brings good fortune to the whole house. In relation\nto general conditions, the counsel here is to seek nothing by means of\nforce, but quietly to confine oneself to the duties at hand.",
        },
        {
          .pd ="When tempers flare up in the family,\nToo great severity brings remorse.\nGood fortune nonetheless.\nWhen woman and chile dally and laugh\nIt leads in the end to humiliation.",
          .cm = "In the family the proper mean between severity and indulgence ought\nto prevail. Too great severity toward one's own flesh and blood leads\nto remorse. The wise thing is to build strong dikes within which complete\nfreedom of movement is allowed each individual. But in doubtful instances\ntoo great severity, despite occasional mistakes, is preferable, because\nit preserves discipline in the family, whereas too great weakness leads\nto disgrace.",
        },
        {
          .pd ="She is the treasure of the house.\nGreat good fortune.",
          .cm = "It is upon the woman of the house that the well-being of the family\ndepends. Well-being prevails when expenditures and income are soundly\nbalanced. This leads to great good fortune. In the sphere of public life,\nthis line refers to the faithful steward whose measures further the general\nwelfare.",
        },
        {
          .pd ="As a king he approaches his family.\nFear not.\nGood fortune.",
          .cm = "A king is the symbol of a fatherly man who is richly endowed in mind.\nHe does nothing to make himself feared; on the contrary, the whole family\ncan trust him, because love governs their intercourse. His character\nof itself exercises the right influence.",
        },
        {
          .pd ="His work commands respect.'\nIn the end good fortune comes.",
          .cm = "In the last analysis, order within the family depends on the character\nof the master of the house. If he cultivates his personality so that\nit works impressively through the force of inner truth, all goes well\nwith the family. In a ruling position one must of his own accord assume\nresponsibility.",
        }
      }
  },
  {
    .number = 38,
    .sk = 0b00101011,
    .name = "Opposition",
    .cm = "This hexagram is composed of the trigram Li above, i.e., flame, which\nburns upward, and Tui below, i.e., the lake, which seeps downward. These\ntwo movements are in direct contrast. Furthermore, LI is the second daughter\nand Tui the youngest daughter, and although they live in the same house\nthey belong to different men; hence their wills are not the same but are\ndivergently directed.",
    .jd = "OPPOSITION. In small matters, good fortune.",
    .j_cm = "When people live in opposition and estrangement they cannot carry\nout a great undertaking in common; their points of view diverge too widely.\nIn such circumstances one should above all not proceed brusquely, for that\nwould only increase the existing opposition; instead, one should limit oneself\nto producing gradual effects in small matters. Here success can still be\nexpected, because the situation is such that the opposition does not preclude\nall agreement. In general, opposition appears as an obstruction, but when\nit represents polarity within a comprehensive whole, it has also its useful\nand important functions. The oppositions of heaven and earth, spirit and\nnature, man and woman, when reconciled, bring about the creation\nand reproduction of life.\n\nIn the world of visible things, the principle of opposites makes\npossible the differentiation by categories through which order is brought\ninto the world.",
    .im =   "Above, fire; below. The lake.\nThe image of OPPOSITION.\nThus amid all fellowship\nThe superior man retains his individuality.",
    .i_cm = "The two elements, fire and water, never mingle but even when in contact\nretain their own natures. So the sutured man is never led into baseness\nor vulgarity through intercourse or community of interests with persons\nof another sort; regardless of all commingling, he will always preserve\nhis individuality.",
        .lines = {
        {
          .pd ="Remorse disappears.\nIf you lose your horse, do not run after it;\nIt will come back of its own accord.\nWhen you see evil people,\nGuard yourself against mistakes.",
          .cm = "Even in times when oppositions prevail, mistakes can be avoided, so\nthat remorse disappears. When opposition begins to manifest itself, a\nman must not try to bring about unity by force, for by so doing he would\nonly achieve the contrary, just as a horse goes farther and farther away\nif one runs after it. It it is one's won horse, one can safely let it go;\nit will come back of its own accord. So too when someone who belongs with\nus is momentarily estranged because of a misunderstanding, he will return\nof his own accord if we leave matters to him. One the other hand, it is\nwell to be cautious when evil men who do not belong with us force themselves\nupon us, again as the result of a misunderstanding. Here the important thing\nis to avoid mistakes. We must not try to shake off these evil men by force;\nthis would give rise to real hostility. We must simply endure them. They\nwill eventually withdraw of their own accord.",
        },
        {
          .pd ="One meets his lord in a narrow street.\nNo blame.",
          .cm = "As a result of misunderstandings, it has become impossible for people\nwho by nature belong together to meet in the correct way. This being\nso, an accidental meeting under informal circumstances may serve the purpose,\nprovided there is an inner affinity between them.",
        },
        {
          .pd ="One sees the wagon dragged back,\nThe oxen halted,\nA man's hair and nose cut off.\nNot a good beginning, but a good end.",
          .cm = "Often it seems tot a man as though everything were conspiring against\nhim. He sees himself checked and hindered in his progress, insulted and\ndishonored. However, he must not let himself be misled; despite this opposition,\nhe must cleave to the man with whom he knows he belongs. Thus, notwithstanding\nthe bad beginning, the matter will end well.",
        },
        {
          .pd ="Isolated through opposition,\nOne meets a like-minded man\nWith whom one can associate in good faith.\nDespite the danger, no blame.",
          .cm = "If a man finds himself in a company of people from whom he is separated\nby an inner opposition, he becomes isolated. But if in such a situation\na man meets someone who fundamentally by the very law of his being, is\nkin to him, and whom he can trust completely, he overcomes all the dangers\nof isolation. His will achieves its aim, and he becomes free of faults.",
        },
        {
          .pd ="Remorse disappears.\nThe companion bits his way through the wrappings.\nIf one goes to him,\nHow could it be a mistake?",
          .cm = "Coming upon a sincere man, one fails to recognize him at first because\nof the general estrangement. However, he bites his way through the wrappings\nthat are causing the separation. When such a companion thus reveals himself\nin his true character, it is one's duty to go to meet him and to work\nwith him.",
        },
        {
          .pd ="Isolated through opposition,\nOne sees one's companion as a pig covered with dirt,\nAs a wagon full of devils.\nFirst one draws a bow against him,\nThen one lays the bow aside.\nHe is not a robber; he will woo at the right time.\nAs one goes, rain falls; then good fortune comes.",
          .cm = "Here the isolation is due to misunderstanding; it is brought about\nnot by outer circumstances but by inner conditions. A man misjudges his\nbest friends, taking them to be as unclean as a dirty pig in and as dangerous\nas a wagon full of devils. He adopts an attitude of defense. But in the\nend, realizing his mistake, he lays aside the bow, perceiving that the\nother is approaching with the best intentions for the purpose of close union.\nThus the tension is relieved. The union resolves the tension, just as falling\nrain relieves the sultriness preceding a thunderstorm. All goes well,\nfor just when opposition reaches its climax it changes over to its antithesis.",
        }
      }
  },
  {
    .number = 39,
    .sk = 0b00010100,
    .name = "Obstruction",
    .cm = "The hexagram pictures a dangerous abyss lying before us and a steep,\ninaccessible mountain rising behind us. We are surrounded by obstacles;\nat the same time, since the mountain has the attribute of keeping still,\nthere is implicit a hint as to how we can extricate ourselves. The hexagram\nrepresents obstructions that appear in the course of time but that can\nand should be overcome. Therefore all the instruction given is directed\nto overcoming them.",
    .jd = "OBSTRUCTION. The southwest furthers.\nThe northeast does not further.\nIt furthers one to see the great man.\nPerseverance brings good fortune.",
    .j_cm = "The southwest is the region of retreat, the northeast that of advance.\nHere an individual is confronted by obstacles that cannot be overcome\ndirectly. In such a situation it is wise to pause in view of the danger\nand to retreat. However, this is merely a preparation for overcoming the\nobstructions. One must join forces with friends of like mind and put himself\nunder the leadership of a man equal to the situation: then one will succeed\nin removing the obstacles. This requires the will to persevere just when\none apparently must do something that leads away from his goal. This unswerving\ninner purpose brings good fortune in the end. An obstruction that lasts\nonly for a time is useful for self-development. This is the value of adversity.",
    .im =   "Water on the mountain:\nThe image of OBSTRUCTION.\nThus the superior man turns his attention to himself\nAnd molds his character.",
    .i_cm = "Difficulties and obstructions throw a man back upon himself. While\nthe inferior man seeks to put the blame on other persons, bewailing his\nfate, the superior man seeks the error within himself, and through this\nintrospection the external obstacle becomes for him an occasion for inner\nenrichment and education.",
        .lines = {
        {
          .pd ="Going leads to obstructions,\nComing meets with praise.",
          .cm = "When one encounters an obstruction, the important thing is to reflect\non how best to deal with it. When threatened with danger, one should\nnot strive blindly to go ahead, for this only leads to complications.\nThe correct thing is, on the contrary, to retreat for the time being,\nnot in order to give up the struggle but to await the right moment for\naction.",
        },
        {
          .pd ="The King's servant is beset by obstruction upon obstruction,\nBut it is not his own fault.",
          .cm = "Ordinarily it is best to go around an obstacle and try to overcome\nit along the line of least resistance. But there is one instance in which\na man must go out to meet the trouble, even though difficulty piles upon\ndifficulty: this is when the path of duty leads directly to it-in other\nwords, when he cannot act of his own volition but is duty bound to go and\nseek out danger in the service of a higher cause. Then he may do it without\ncompunction, because it is not through any fault of his that he is putting\nhimself in this difficult situation.",
        },
        {
          .pd ="Going leads to obstructions;\nHence he comes back.",
          .cm = "While the preceding line shows the official compelled by duty to follow\nthe way of danger, this line shows the man who must act as father of\na family or as head of his kin. If he were to plunge recklessly in to\ndanger, it would be a useless act, because those entrusted to his care\ncannot get along by themselves. But if he withdraws and turns back to his\nown, they welcome him with great joy.",
        },
        {
          .pd ="Going leads to obstructions,\nComing leads to union.",
          .cm = "This too describes a situation that cannot be managed single-handed.\nIn such a case the direct way is not the shortest. If a person were to\nforge ahead on his own strength and without the necessary preparations,\nhe would not find the support he needs and would realize too late that\nhe has been mistaken in his calculations, inasmuch as the conditions on\nwhich he hoped he could rely would prove to be inadequate. In this case\nit is better, therefore, to hold back for the time being and to gather\ntogether trustworthy companions who can be counted upon for help in overcoming\nthe obstructions.",
        },
        {
          .pd ="In the midst of the greatest obstructions,\nFriends come.",
          .cm = "Here we see a man who is called to help in an emergency. He should\nnot seek to evade the obstructions, no matter how dangerously they pile\nup before him. But because he is really called to the task, the power\nof his spirit is strong enough to attract helpers whom he can effectively\norganize, so that through the well-directed co-operation of all participants\nthe obstruction is overcome.",
        },
        {
          .pd ="Going leads to obstructions,\nComing leads to great good fortune.\nIt furthers one to see the great man.",
          .cm = "This refers to a man who has already left the world and its tumult\nbehind him. When the time of obstructions arrives, it might seem that\nthe simplest thing for him to do would be to turn his back upon the world\nand take refuge in the beyond. But this road is barred to him. He must\nnot seek his own salvation and abandon the world to its adversity. Duty\ncalls him back once more into the turmoil of life. Precisely because of\nhis experience and inner freedom, he is able to create something both great\nand complete that brings good fortune. And it is favorable to see the great\nman in alliance with whom one can achieve the work of rescue.",
        }
      }
  },
  {
    .number = 40,
    .sk = 0b00001010,
    .name = "Deliverance",
    .cm = "Here the movement goes out of the sphere of danger. The obstacle has\nbeen removed, the difficulties are being resolved. Deliverance is not\nyet achieved; it is just in its beginning, and the hexagram represents\nits various stages.",
    .jd = "DELIVERANCE. The southwest furthers.\nIf there is no longer anything where one has to go,\nReturn brings good fortune.\nIf there is still something where one has to go,\nHastening brings good fortune.",
    .j_cm = "This refers to a time in which tensions and complications begin to\nbe eased. At such times we ought to make our way back to ordinary conditions\nas soon as possible; this is the meaning of \"the southwest.\" These periods\nof sudden change have great importance. Just as rain relieves atmospheric\ntension, making all the buds burst open, so a time of deliverance from\nburdensome pressure has a liberating and stimulating effect on life.\nOne thing is important, however: in such times we must not overdo our triumph.\nThe point is not to push on farther than is necessary. Returning to the\nregular order of life as soon as deliverance is achieved brings good fortune.\nIf there are any residual matters that ought to be attended to, it should\nbe done as quickly as possible,\nso that a clean sweep is made and no retardations occur.",
    .im =   "Thunder and rain set in:\nThe image of DELIVERANCE.\nThus the superior man pardons mistakes\nAnd forgives misdeeds.",
    .i_cm = "A thunderstorm has the effect of clearing the air; the superior man\nproduces a similar effect when dealing with mistakes and sins of men\nthat induce a condition of tension. Through clarity he brings deliverance.\nHowever, when failings come to light, he does not dwell on them; he simply\npasses over mistakes, the unintentional transgressions, just as thunder\ndies away. He forgives misdeeds, the intentional transgressions, just as\nwater washes everything clean.",
        .lines = {
        {
          .pd ="Without blame.",
          .cm = "In keeping with the situation, few words are needed. The hindrance\nis past, deliverance has come. One recuperates in peace and keeps still.\nThis is the right thing to do in times when difficulties have been overcome.",
        },
        {
          .pd ="One kills three foxes in the field\nAnd receives a yellow arrow.\nPerseverance brings good fortune.",
          .cm = "The image is taken from the hunt. The hunter catches three cunning\nfoxes and receives a yellow arrow as a reward. The obstacles in public\nlife are the designing foxes who try to influence the ruler through flattery.\nThey must be removed before there can be any deliverance. But the struggle\nmust not be carried on with the wrong weapons. The yellow color points\nto measure and mean in proceeding against the enemy; the arrow signifies\nthe straight course. If one devotes himself wholeheartedly to the task\nof deliverance, he develops so much inner strength from his rectitude\nthat it acts as a weapon against all that is false and low.",
        },
        {
          .pd ="If a man carries a burden on his back\nAnd nonetheless rides in a carriage,\nHe thereby encourages robbers to draw near.\nPerseverance leads to humiliation.",
          .cm = "This refers to a man who has come out of needy circumstances in to\ncomfort and freedom from want. If now, in the manner of an upstart, he\ntries to take his ease in comfortable surroundings that do not suit his\nnature, he thereby attracts robbers. If he goes on thus he is sure to\nbring disgrace upon himself. Confucius says about this line:\nCarrying a burden on the back is the business of common man; a carriage\nis the appurtenance of a man of rank. Now, when a common man uses the\nappurtenance of man of rank, robbers plot to take it away from him. If\na man is insolent toward those above him and hard toward those below him,\nrobbers plot to attack him. Carelessness in guarding things tempts thieves\nto steal. Sumptuous ornaments worn by a maiden are an enticement to rob\nher of her virtue.",
        },
        {
          .pd ="Deliver yourself from your great toe.\nThen the companion comes,\nAnd him you can trust.",
          .cm = "In times of standstill it will happen that inferior people attach\nthemselves to a superior man, and through force of daily habit they may grow\nvery close to him and become indispensable, just as the big toe is indispensable\nto the foot because it makes walking easier. But when the time of deliverance\ndraws near, with its call to deeds, a man must free himself from such\nchance acquaintances with whim he has no inner connection. For otherwise\nthe friends who share his views, on whom he could really rely and together\nwith whom he could accomplish something, mistrust him and stay away.",
        },
        {
          .pd ="If only the superior man can deliver himself,\nIt brings good fortune.\nThus he proves to inferior men that he is in earnest.",
          .cm = "Times of deliverance demand inner resolve. Inferior people cannot\nbe driven off by prohibitions or any external means. If one desires to be\nrid of them, he must first break completely with them in his own mind; they\nwill see for themselves that he is in earnest and will withdraw.",
        },
        {
          .pd ="The prince shoots at a hawk on a high wall.\nHe kills it. Everything serves to further.",
          .cm = "The hawk on a high wall is the symbol of a powerful inferior in a\nhigh position who is hindering the deliverance. He withstands the force of\ninner influences, because he is hardened in his wickedness. He must be forcibly\nremoved, and this requires appropriate means.\nConfucius says about this line:\nThe hawk is the object of the hunt; bow and arrow are the tools and\nmeans. The marksman is man (who must make proper use of the means to\nhis end). The superior man contains the means in his own person. He bides\nhis time and then acts. Why then should not everything go well? He acts\nand is free. Therefore all he has to do is to go forth, and he takes his\nquarry. This is how a man fares who acts after he has made ready the means.",
        }
      }
  },
  {
    .number = 41,
    .sk = 0b00100011,
    .name = "Decrease",
    .cm = "This hexagram represents a decrease of the lower trigram in favor\nof the upper, because the third line, originally strong, has moved up to\nthe top, and the top line, originally weak, has replaced it. What is below\nis decreased to the benefit of what is above. This is out-and-out decrease.\nIf the foundations of a building are decreased in strength and the upper\nwalls are strengthened, the whole structure loses its stability. Likewise,\na decrease in the prosperity of the people in favor of the government is\nout-and-out decrease. And the entire theme of the hexagram is directed to\nshowing how this shift of wealth can take place without causing the sources\nof wealth can take place without causing the sources of wealth in the nation\nand its lower classes to fail.",
    .jd = "DECREASE combined with sincerity\nBrings about supreme good fortune\nWithout blame.\nOne may be persevering in this.\nIt furthers one to undertake something.\nHow is this to be carried out?\nOne may use two small bowls for the sacrifice.",
    .j_cm = "Decrease does not under all circumstances mean something bad. Increase\nand decrease come in their own time. What matters here is to understand\nthe time and not to try to cover up poverty with empty pretense. If a\ntime of scanty resources brings out an inner truth, one must not feel\nashamed of simplicity. For simplicity is then the very thing needed to\nprovide inner strength for further undertakings. Indeed, there need by\nno concern if the outward beauty of the civilization, even the elaboration\nof religious forms, should have to suffer because of simplicity. One must\ndraw on the strength of the inner attitude to compensate for what is lacking\nin externals; then the power of the content makes up for the simplicity\nof form. There is no need of presenting false appearances to God. Even\nwith slender means, the sentiment of the heart can be expressed.",
    .im =   "At the foot of the mountain, the lake:\nThe image of DECREASE.\nThus the superior man controls his anger\nAnd restrains his instincts.",
    .i_cm = "The lake at the foot of the mountain evaporates. In this way it decreases\nto the benefit of the mountain, which is enriched by its moisture. The\nmountain stands as the symbol of stubborn strength that can harden into\nanger. The lake is the symbol of unchecked gaiety that can develop into\npassionate drives at the expense of the life forces. Therefore decrease\nis necessary; anger must be decreased by keeping still, the instincts must\nbe curbed by restriction. By this decrease of the lower powers of the psyche,\nthe higher aspects of the soul are enriched/",
        .lines = {
        {
          .pd ="Going quickly when one's tasks are finished\nIs without blame.\nBut one must reflect on how much one may decrease others.",
          .cm = "It is unselfish and good when a man, after completing his own urgent\ntasks, uses his strength in the service of others, and without bragging\nor making much of it, helps quickly where help is needed. But the man\nin a superior position who is thus aided must weigh carefully how much\nhe can accept without doing the helpful servant or friend real harm. Only\nwhere such delicacy of feeling exists can one give oneself unconditionally\nand without hesitation.",
        },
        {
          .pd ="Perseverance furthers.\nTo undertake something brings misfortune.\nWithout decreasing oneself,\nOne is able to bring increase to others.",
          .cm = "A high-minded self-awareness and a consistent seriousness with no\nforfeit of dignity are necessary if a man wants to be of service to others.\nHe who throw himself away in order to do the bidding of a superior diminishes\nhis own position without thereby giving lasting benefit to the other. This\nis wrong. To render true service of lasting value to another, one must serve\nhim without relinquishing oneself.",
        },
        {
          .pd ="When three people journey together,\nTheir number increases by one.\nWhen one man journeys alone,\nHe finds a companion.",
          .cm = "When there are three people together, jealousy arises. One of them\nwill have to go. Avery close bond is possible only between two people.\nBut when one man is lonely, he is certain to find a companion who complements\nhim.",
        },
        {
          .pd ="If a man deceases his faults,\nIt makes the other hasten to come and rejoice.\nNo blame.",
          .cm = "A man's faults often prevent even well-disposed people from coming\ncloser to him. His faults are sometimes reinforced by the environment\nin which he lives. But if in humility he can bring himself to the point\nof giving them up, he frees his well-disposed friends from an inner pressure\nand causes them to approach the more quickly, and there is mutual joy.",
        },
        {
          .pd ="Someone does indeed increase him.\nTen pairs of tortoises cannot oppose it.\nSupreme good fortune.",
          .cm = "If someone is marked out by fate for good fortune, it comes without\nfail. All oracles-as for instance those that are read from the shells\nof tortoises-are bound to concur in giving him favorable signs. He need\nfear nothing, because his luck is ordained from on high.",
        },
        {
          .pd ="If one is increased without depriving other,\nThere is no blame.\nPerseverance brings good fortune.\nIt furthers one to undertake something.\nOne obtains servants\nBut no longer has a separate home.",
          .cm = "There are people who dispense blessings to the whole world. Every\nincrease in power that comes to them benefits the whole of mankind and therefore\ndoes not bring decrease to others. Through perseverance and zealous work\na man wins success and finds helpers as they are needed. But what he accomplishes\nis not a limited private advantage; it is a public good and available\nto everyone.",
        }
      }
  },
  {
    .number = 42,
    .sk = 0b00110001,
    .name = "Increase",
    .cm = "The idea of increase is expressed in the fact that the strong lowest\nline of the upper trigram has sunk down and taken its place under the\nlower trigram. This conception also expresses the fundamental idea on\nwhich the Book of Changes is based. To rule truly is to serve. A sacrifice\nof the higher element that produces an increase of the lower is called\nan out-and-out increase: it indicates the spirit that alone has power to\nhelp the world.",
    .jd = "INCREASE. It furthers one\nTo undertake something.\nIt furthers one to cross the great water.",
    .j_cm = "Sacrifice on the part of those above for the increase of those below\nfills the people with a sense of joy and gratitude that is extremely\nvaluable for the flowering of the commonwealth. When people are thus\ndevoted to their leaders, undertakings are possible, and even difficult\nand dangerous enterprises will succeed. Therefore in such times of progress\nand successful development it is necessary to work and make the best use\nof time. This time resembles that of the marriage of heaven and earth,\nwhen the earth partakes of the creative power of heaven, forming and bringing\nforth living beings. The time of INCREASE does not endure, therefore it\nmust be utilized while it lasts.",
    .im =   "Wind and thunder: the image of INCREASE.\nThus the superior man:\nIf he sees good, he imitates it;\nIf he has faults, he rids himself of them.",
    .i_cm = "While observing how thunder and wind increase and strengthen each\nother, a man can not the way to self-increase and self-improvement. When\nhe discovers good in others, he should imitate it and thus make everything\non earth his own. If he perceives something bad in himself, let him rid himself\nof it. In this way he becomes free of evil. This ethical change represents\nthe most important increase of personality.",
        .lines = {
        {
          .pd ="It furthers one to accomplish great deeds.\nSupreme good fortune. No blame.",
          .cm = "If great help comes to a man from on high, this increased strength\nmust be used to achieve something great for which he might otherwise\nnever have found energy, or readiness to take responsibility. Great good\nfortune is produced by selflessness, and in bringing about great good\nfortune, he remains free of reproach.",
        },
        {
          .pd ="Someone does indeed increase him;\nTen pairs of tortoises cannot oppose it.\nConstant perseverance brings good fortune.\nThe king presents him before God.\nGood fortune.",
          .cm = "A man brings about real increase by producing in himself the conditions\nfor it, that it, through receptivity to and love of the good. Thus the\nthing for which he strives comes of itself, with the inevitability of\nnatural law. Where increase is thus in harmony with the highest laws of\nthe universe, it cannot be prevented by any constellation of accidents.\nBut everything depends on his not letting unexpected good fortune make\nhim heedless; he must make it his own through inner strength an steadfastness.\nThen he acquires meaning before God and man, and can accomplish something\nfor the good of the world.",
        },
        {
          .pd ="One is enriched through unfortunate events.\nNo blame, if you are sincere\nAnd walk in the middle,\nAnd report with a seal to the prince.",
          .cm = "A time of blessing and enrichment has such powerful effects that even\nevents ordinarily unfortunate must turn out to the advantage of those\naffected by them. These persons become free of error, and by acting in\nharmony with truth they gain such inner authority that they exert influence\nas if sanctioned by the letter and seal.",
        },
        {
          .pd ="If you walk in the middle\nAnd report the prince,\nHe will follow.\nIt furthers one to be used\nIn the removal of the capital.",
          .cm = "It is important that there should be men who mediate between leaders\nand followers. These should be disinterested people, especially in times\nof increase, since the benefit is to spread from the leader to the people.\nNothing of this benefit should be held back in a selfish way; it should\nreally reach those for whom it is intended. This sort of intermediary,\nwho also exercises a good influence on the leader, is especially important\nin times when it is a matter of great undertakings, decisive for the future\nand requiring the inner assent of all concerned.",
        },
        {
          .pd ="If in truth you have a kind heart, ask not.\nSupreme good fortune.\nTruly, kindness will be recognized as your virtue.",
          .cm = "True kindness does not count upon nor ask about merit and gratitude\nbut acts from inner necessity. And such a truly kind heart finds itself\nrewarded in being recognized, and thus the beneficent influence will spread\nunhindered.",
        },
        {
          .pd ="He brings increase to no one.\nIndeed, someone even strikes him.\nHe does not keep his heart constantly steady.\nMisfortune.",
          .cm = "The meaning here is that through renunciation those in high place\nshould bring increase to those below. By neglecting this duty and helping\nno one, they in turn lose the furhtering influence of others and soon find\nthemselves alone. In this way they invite attacks. An attitude not permanently\nin harmony with the demands of the time will necessarily bring misfortune\nwith it. Confucius says about this line:\nThe superior man sets his person at rest before he moves; he composes\nhis mind before he speaks; he makes his relations firm before he asks\nfor something. By attending to these three matters, the superior man gains\ncomplete security. But if a man is brusque in his movements, others will\nnot cooperate. If he is agitated in his word, they awaken no echo in others.\nIf he asks for something without having fist established relations, it\nwill not be given to him. If no one is with him, those who would harm\nhim draw near.",
        }
      }
  },
  {
    .number = 43,
    .sk = 0b00011111,
    .name = "Break-through (Resoluteness)",
    .cm = "This hexagram signifies on the one hand a break-through after a long\naccumulation of tension, as a swollen river breaks through its dikes,\nor in the manner of a cloudburst.\n\nOn the other hand, applied to human conditions, it refers\nto the time when inferior people gradually begin to disappear.\nTheir influence is on the wane; as a result of resolute action, a change\nin conditions occurs, a break-through. The hexagram is linked with the\nthird month [April-May].",
    .jd = "BREAK-THROUGH. \nOne must resolutely make the matter known\nAt the court of the king.\nIt must be announced truthfully. Danger.\nIt is necessary to notify one's own city.\nIt does not further to resort to arms.\nIt furthers one to undertake something.",
    .j_cm = "Even if only one inferior man is occupying a ruling position in a\ncity, he is able to oppress superior men. Even a single passion still lurking\nin the heart has power to obscure reason. Passion and reason cannot exist\nside by side-therefore fight without quarter is necessary if the good is\nto prevail. In a resolute struggle of the good against evil, there are,\nhowever, definite rules that must not be disregarded, if it is to succeed.\nFirst, resolution must be based on a union of strength and friendliness.\nSecond, a compromise with evil is not possible; evil must ALWAYS\nbe openly discredited. Nor must our own passions and shortcomings be glossed\nover. Third, the struggle must not be carried on directly by force. If evil\nis branded, it thinks of weapons, and if we do it the favor of fighting against\nit blow for blow, we lose in the end because thus we ourselves get entangled\nin hatred and passion. Therefore it is important to begin at home, to be\non guard in our own persons against the faults we have branded. In this way,\nfinding no opponent, the sharp edges of the weapons of evil becomes dulled.\nFor the same reasons we should not combat our own faults directly. As long\nas we wrestle with them, they continue victorious. Finally, the best way\nto fight evil is to make energetic progress in the good.",
    .im =   "The lake has risen up to heaven:\nThe image of BREAK-THROUGH.\nThus the superior man\nDispenses riches downward\nAnd refrains from resting on his virtue.",
    .i_cm = "When the water of a lake has risen up to heaven, there is reason to\nfear a cloudburst. Taking this as a warning, the superior man forestalls\na violent collapse. If a man were to pile up riches for himself alone,\nwithout considering others, he would certainly experience a collapse.\nIf a man were to pile up riches for himself alone, without considering\nothers, he would certainly experience a collapse. For all gathering is\nfollowed by dispersion. Therefore the superior man begins to distribute\nwhile he is accumulating. In the same way, in developing his character\nhe takes care not to become hardened in obstinacy but to remain receptive\nto impressions by help of strict and continuous self-examination.",
        .lines = {
        {
          .pd ="Mighty in the forward-striding toes.\nWhen one goes and is not equal to the task,\nOne makes a mistake.",
          .cm = "In times of resolute advance, the beginning is especially difficult.\nWe feel inspired to press forward but resistance is still strong; therefore\nwe ought to gauge our own strength and venture only so far as we can\ngo with certainty of success. To plunge blindly ahead is wrong, because\nit is precisely at the beginning that an unexpected setback can have the\nmost disastrous results.",
        },
        {
          .pd ="A cry of alarm. Arms at evening and at night.\nFear nothing.",
          .cm = "Readiness is everything. Resolution is indissolubly bound up with\ncaution. If an individual is careful and keeps his wits about him, he need\nnot become excited or alarmed. If he is watchful at all times, even before\ndanger is present, he is armed when danger approaches and need not be afraid.\nThe superior man is on his guard against what is not yet in sight and on\nthe alert for what is not yet within hearing; therefore he dwells in the\nmidst of difficulties as thought hey did not exist. If a man develops his\ncharacter, people submit to him of their own accord. If reason triumphs,\nthe passions withdraw of themselves. To be circumspect and not to forget\none's armor is the right way to security.",
        },
        {
          .pd ="To be powerful in the cheekbones\nBrings misfortune.\nThe superior man is firmly resolved.\nHe walks alone and is caught in the rain.\nHe is bespattered,\nAnd people murmur against him.\nNo blame.",
          .cm = "Here we have a man in an ambiguous situation. While all others are\nengaged in a resolute fight against all that is inferior, he alone has\na certain relationship with an inferior man. If he were to show strength\noutwardly and turn against this man before the time is ripe, he would only\nendanger the entire situation, because the inferior man would too quickly\nhave recourse to countermeasures. The task of the superior man becomes\nextremely difficult here. He must be firmly resolved within himself and,\nwhile maintaining association with the inferior man, avoid any participation\nin his evilness. He will of course be misjudged. It will be thought that\nhe belong to the party of the inferior man. He will be lonely because no\none will understand him. His relations with the inferior man will sully\nhim in the eyes of the multitude, and they will turn against him, grumbling.\nBut he can endure this lack of appreciation and makes no mistake, because\nhe remains true to himself.",
        },
        {
          .pd ="There is no skin on his thighs,\nAnd walking comes hard.\nIf a man were to let himself be led like a sheep,\nRemorse would disappear.\nBut if these words are heard\nThey will not be believed.",
          .cm = "Here a man is suffering from inner restlessness and cannot abide in\nhis place. He would like to push forward under any circumstances, but\nencounters insuperable obstacles. Thus his situation entails an inner\nconflict. This is due to the obstinacy with which he seeks to enforce\nhis will. If he would desist from this obstinacy, everything would go\nwell. But this advice, like so much other good counsel, will be ignored.\nFor obstinacy makes a man unable to hear, for all that he has ears.",
        },
        {
          .pd ="In dealing with weeds,\nFirm resolution is necessary.\nWalking in the middle\nRemains free of blame.",
          .cm = "Weeds always grow back again and are difficult to exterminate. So\ntoo the struggle against an inferior man in a high position demands firm\nresolution. One has certain relations with him, hence there is danger that\none may give up the struggle as hopeless. But this must not be. One must\ngo on resolutely and not allow himself to be deflected from him course. Only\nin this way does one remain free of blame.",
        },
        {
          .pd ="No cry.\nIn the end misfortune comes.",
          .cm = "Victory seems to have been achieved. There remains merely a remnant\nof the evil resolutely to be eradicated as the time demands. Everything\nlooks easy. Just there, however, lies the danger. If we are not on guard,\nevil will succeed in escaping by means of concealment, and when it has\neluded us new misfortunes will develop from the remaining seeds, for evil\ndoes not die easily. So too in dealing with the evil in own's own character,\none must go to work with thoroughness. If out of carelessness anything were\nto be overlooked, new evil would arise from it.",
        }
      }
  },
  {
    .number = 44,
    .sk = 0b00111110,
    .name = "Coming to Meet",
    .cm = "This hexagram indicates a situation in which the principle of darkness,\nafter having been eliminated, furtively and unexpectedly obtrudes again\nfrom within and below. Of its own accord the female principle comes to\nmeet the male. It is an unfavorable and dangerous situation, and we must\nunderstand and promptly prevent the possible consequences. The hexagram\nis linked with the fifth month [June-July], because at the summer solstice\nthe principle of darkness gradually becomes ascendant again.",
    .jd = "COMING TO MEET. The maiden is powerful.\nOne should not marry such a maiden.",
    .j_cm = "The rise of the inferior element is pictured here in the image of\na bold girl who lightly surrenders herself and thus seizes power. This would\nnot be possible if the strong and light-giving element had not in turn\ncome halfway. The inferior thing seems so harmless and inviting that a\nman delights in it; it looks so small and weak that he imagines he may\ndally with it and come to no harm. The inferior man rises only because\nthe superior man does not regard him as dangerous and so lends him power.\nIf he were resisted from the fist, he could never gain influence. The time\nof COMING TO MEET is important in still another way. Although as a general\nrule the weak should not come to meet the strong, there are times when\nthis has great significance. When heaven and earth come to meet each other,\nall creatures prosper; when a prince and his official come to meet each\nother, the world is put in order. It is necessary for elements predestined\nto be joined and mutually dependent to come to meet one another halfway.\nBut the coming together must be free of dishonest ulterior motives, otherwise\nharm will result.",
    .im =   "Under heaven, wind:\nThe image of COMING TO MEET.\nThus does the prince act when disseminating his commands\nAnd proclaiming them to the four quarters of heaven.",
    .i_cm = "The situation here resembles that in hexagram 20, Kuan, CONTEMPLATION\n( VIEW). In the latter the wind blows over the earth, here it blows under\nheaven; in both cases it goes everywhere. There the wind is on the earth\nand symbolizes the ruler taking note of the conditions in his kingdom;\nhere the wind blows from above and symbolizes the influence exercised by\nthe ruler through his commands. Heaven is far from the things of earth,\nbut it sets them in motion by means of the wind. The ruler is far form\nhis people, but he sets them in motion by means of his commands and decrees.",
        .lines = {
        {
          .pd ="It must be checked with a brake of bronze.\nPerseverance brings good fortune.\nIf one lets it take its course, one experiences misfortune.\nEven a lean pig has it in him to rage around.",
          .cm = "If an inferior element has wormed its way in, it must be energetically\nchecked at once. By consistently checking it, bad effects can be avoided.\nIf it is allowed to take its course, misfortune is bound to result; the\ninsignificance of that which creeps in should not be a temptation to underrate\nit. A pig that is still young and lean cannot rage around much, but after\nit has eaten its fill and become strong, its true nature comes out if\nit has not previously been curbed.",
        },
        {
          .pd ="There is a fish in the tank. No blame.\nDoes not further guests.",
          .cm = "The inferior element is not overcome by violence but is kept under\ngentle control. Then nothing evil is to be feared. But care must be taken\nnot to let it come in contact with those further away, because once free\nit would unfold its evil aspects unchecked.",
        },
        {
          .pd ="There is no skin on his thighs,\nAnd walking comes hard.\nIf one is mindful of the danger,\nNo great mistake is made.",
          .cm = "There is a temptation to fall in with the evil element offering itself-a\nvery dangerous situation. Fortunately circumstances prevent this; one\nwould like to do it, but cannot. This leads to painful indecision in behavior.\nBut if we gain clear insight into the danger of the situation, we shall\nat least avoid more serious mistakes.",
        },
        {
          .pd ="No fish in the tank.\nThis leads to misfortune.",
          .cm = "Insignificant people must be tolerated in order to keep them well\ndisposed. Then we can make use of them if we should need them. If we become\nalienated from them and do not meet them halfway, they turn their backs on\nus and are not at our disposal when we need them. But this is our own fault.",
        },
        {
          .pd ="A melon covered with willow leaves.\nHidden lines.\nThen it drops down to one from heave.",
          .cm = "The melon, like the fish, is a symbol of the principle of darkness.\nIt is sweet but spoils easily and for this reason is protected with a\ncover of willow leaves. This is a situation in which a strong, superior,\nwell-poised man tolerates and protects the inferiors in his charge. He\nhas the firm lines of order an beauty within himself but he does not lay\nstress upon them. He does not bother his subordinates with outward show\nor tiresome admonitions but leaves them quite free, putting his trust in\nthe transforming power of a strong and upright personality. And behold!\nFate is favorable. His inferiors respond to his influence and fall to his\ndisposition like ripe fruit.",
        },
        {
          .pd ="He comes to meet with his horns.\nHumiliation. No blame.",
          .cm = "When a man has withdrawn from the world, its tumult often becomes\nunbearable to him. There are many people who in a noble pride hold themselves\naloof from all that is low and rebuff it brusquely wherever it comes to meet\nthem. Such persons are reproached for being proud and distant, but since\nactive duties no longer hold them to the world, this does not greatly matter.\nThey know how to bear the dislike of the masses with composure.",
        }
      }
  },
  {
    .number = 45,
    .sk = 0b00011000,
    .name = "Gathering Together (Massing)",
    .cm = "This hexagram is related in form and meaning to Pi, HOLDING TOGETHER\n8. In the latter, water is over the earth; here a\nlake is over the earth. But since the lake is a place where water collects,\nthe idea of gathering together is even more strongly expressed here than\nin the other hexagram. The same idea also arises from the fact that in\nthe present case it is two strong lines (the fourth and the fifth) that\nbring about the gather together, whereas in the former case one strong\nline (the fifth) stands in the midst of weak lines.",
    .jd = "GATHERING TOGETHER. Success.\nThe king approaches his temple.\nIt furthers one to see the great man.\nThis brings success. Perseverance furthers.\nTo bring great offerings creates good fortune.\nIt furthers one to undertake something.",
    .j_cm = "The gathering together of people in large communities is either a\nnatural occurrence, as in the case of the family, or an artificial one, as\nin the case of the state. The family gathers about the father as its head.\nThe perpetuation of this gathering in groups is achieved through the sacrifice\nto the ancestors, at which the whole clan is gathered together. Through the\ncollective piety of the living members of the family, the ancestors become\nso integrated in the spiritual life of the family that it cannot be dispersed\nor dissolved. Where men are to be gathered together, religious forces are\nneeded. But there must also be a human leader to serve as the center of the\ngroup. In order to be able to bring others together, this leader must first\nof all be collected within himself. Only collective moral force can unite\nthe world. Such great times of unification will leave great achievements\nbehind them. This is the significance of the great offerings that are made.\nIn the secular sphere likewise there is no need of great deeds in the time\nof GATHERING TOGETHER.",
    .im =   "Over the earth, the lake:\nThe image of GATHERING TOGETHER.\nThus the superior man renews his weapons\nIn order to meet the unforeseen.",
    .i_cm = "If the water in the lake gathers until it rises above the earth, there\nis danger of a break-through. Precautions must be taken to prevent this.\nSimilarly where men gather together in great numbers, strife is likely\nto arise; where possessions are collected, robbery is likely to occur.\nThus in the time of GATHERING TOGETHER we must arm promptly to ward off\nthe unexpected. Human woes usually come as a result of unexpected events\nagainst which we are not forearmed. If we are prepared, they can be prevented.",
        .lines = {
        {
          .pd ="If you are sincere, but not to the end,\nThere will sometimes be confusion, sometimes gathering together.\nIf you call out,\nThen after one grasp of the hand you can laugh again.\nRegret not. \nGoing is without blame.",
          .cm = "The situation is this: People desire to gather around a leader to\nwhom they look up. But they are in a large group, by which they allow\nthemselves to be influenced, so that they waver in their decision. Thus\nthey lack a firm center around which to gather. But if expression is given\nto this need, and if they call for help, one grasp of the hand from the\nleader is enough to turn away all distress. Therefore they must not allow\nthemselves to be led astray. It is undoubtedly right that they should attach\nthemselves to this leader.",
        },
        {
          .pd ="Letting oneself be drawn\nBrings good fortune and remains blameless.\nIf one is sincere,\nIt furthers one to bring even a small offering.",
          .cm = "In the time of GATHERING TOGETHER, we should make no arbitrary choice\nof the way. There are secret forces at work, leading together those who\nbelong together. We must yield to this attraction; then we make no mistakes.\nWhere inner relationships exist, no great preparations and formalities\nare necessary. People understand one another forthwith, just as the Divinity\ngraciously accepts a small offering if it comes from the heart.",
        },
        {
          .pd ="Gathering together amid sighs.\nNothing that would further.\nGoing is without blame.\nSlight humiliation.",
          .cm = "Often a man feels an urge to unite with others, but the individuals\naround him have already formed themselves into a group, so that he remains\nisolated. The whole situation proves untenable. Then he ought to choose\nthe way of progress, resolutely allying himself with a man who stands\nnearer to the center of the group, and can help him to gain admission\nto the closed circle. This is not a mistake, even though at first his\nposition as an outsider is somewhat humiliating.",
        },
        {
          .pd ="Great good fortune. No blame.",
          .cm = "This describes a man who gathers people around him in the name of\nhis ruler. Since he is not striving for any special advantages for himself\nbut is working unselfishly to bring about general unity, his work is crowned\nwith success, and everything becomes as it should be.",
        },
        {
          .pd ="If in gathering together one has position,\nThis brings no blame.\nIf there are some who are not yet sincerely in the work,\nSublime and enduring perseverance is needed.\nThen remorse disappears.",
          .cm = "When people spontaneously gather around a man, it is only a good.\nIt gives him a certain influence that can be altogether useful.. But of course\nthere is also the possibility that many may gather around him not because\nof a feeling of confidence but merely because of his influential position.\nThis is certainly to be regretted. The only means of dealing with such\npeople is to gain their confidence through steadfastness an intensified,\nunswerving devotion to duty. In this way secret mistrust will gradually\nbe overcome, and there will be no occasion for regret.",
        },
        {
          .pd ="Lamenting and sighing, floods of tears.\nNo blame.",
          .cm = "It may happen that an individual would like to ally himself with another,\nbut his good intentions are misunderstood. Then he becomes sad and laments.\nBut this is the right course. For it may cause the other person to come\nto his senses, so that the alliance that has been sought and so painfully\nmissed is after all achieved.",
        }
      }
  },
  {
    .number = 46,
    .sk = 0b00000110,
    .name = "Pushing Upward",
    .cm = "The lower trigram, Sun, represents wood, and the upper, K'un, means\nthe earth. Linked with this is the idea that wood in the earth grows\nupward. In contrast to the meaning of Chin, PROGRESS 35, this pushing upward is associated with effort,\njust as a plant needs energy for pushing upward through the earth. That\nis why this hexagram, although it is connected with success, is associated\nwith effort of the will. In PROGRESS the emphasis is on expansion; PUSHING\nUPWARD indicates rather a vertical ascent-direct rise from obscurity and\nlowliness to power and influence.",
    .jd = "PUSHING UPWARD has supreme success.\nOne must see the great man.\nFear not.\nDeparture toward the south\nBrings good fortune.",
    .j_cm = "The pushing upward of the good elements encounters no obstruction\nand is therefore accompanied by great success. The pushing upward is made\npossible not by violence but by modesty and adaptability. Since the individual\nis borne along by the propitiousness of the time, he advances. He must go\nto see authoritative people. He need not be afraid to do this, because success\nis assured. But he must set to work, for activity (this is the meaning\nof \"the south\") brings good fortune.",
    .im =   "Within the earth, wood grows:\nThe image of PUSHING UPWARD.\nThus the superior man of devoted character\nHeaps up small things\nIn order to achieve something high and great.",
    .i_cm = "Adapting itself to obstacles and bending around them, wood in the\nearth grows upward without haste and without rest. Thus too the superior\nman is devoted in character and never pauses in his progress.",
        .lines = {
        {
          .pd ="Pushing upward that meets with confidence\nBrings great good fortune.",
          .cm = "This situation at the beginning of ascent. Just as wood draws strength\nfor its upward push from the root, which in itself is in the lowest place,\nso the power to rise comes from this low and obscure station. But there\nis a spiritual affinity with the rulers above, and this solidarity creates\nthe confidence needed to accomplish something.",
        },
        {
          .pd ="If one is sincere,\nIt furthers one to bring even a small offering.\nNo blame.",
          .cm = "Here a strong man is presupposed. It is true that he does not fit\nin with his environment, inasmuch as he is too brusque and pays too little\nattention to form. But as he is upright in character, he meets with response,\nand his lack of outward form does no harm. Here uprightness is the outcome\nof sound qualities of character, whereas in the corresponding line of the\npreceding hexagram it is the result of innate humility.",
        },
        {
          .pd ="One pushes upward into an empty city.",
          .cm = "All obstructions that generally block progress fall away here. Things\nproceed with remarkable ease. Unhesitatingly one follows this road, in\norder to profit by one's success. Seen from without, everything seems\nto be in the best of order. However, no promise of good fortune is added.\nIt is a question how long such unobstructed success can last. But it is\nwise not to yield to such misgivings, because they only inhibit one's power.\nInstead, the point is to profit by the propitiousness of time.",
        },
        {
          .pd ="The king offers him Mount Ch'i.\nGood fortune. No blame.",
          .cm = "Mount Ch'i is in the western China, the homeland of King Wên,\nwhose son, the Duke of Chou, added the words to the individual lines.\nThe pronouncement takes us back to a time when the Chou dynasty was coming\ninto power. At that time King Wên introduced his illustrious helpers\nto the god of his native mountain, and they received their places in the\nhalls of the ancestors by the side of the ruler. This indicates a stage\nin which pushing upward attains its goal. One acquires fame in the sight\nof gods and men, is received into the circle of those who foster the spiritual\nlife of the nation, and thereby attains a significance that endures beyond\ntime.",
        },
        {
          .pd ="Perseverance brings good fortune.\nOne pushes upward by steps.",
          .cm = "When a man is advancing farther and farther, it is important for him\nnot to become intoxicated by success. Precisely when he experiences great\nsuccess it is necessary to remain sober and not to try to skip any stages;\nhe must go on slowly, step by step, as though hesitant. Only such calm,\nsteady progress, overleaping nothing, leads to the goal.",
        },
        {
          .pd ="Pushing upward in darkness.\nIt furthers one\nTo be unremittingly persevering.",
          .cm = "He who pushes upward blindly deludes himself. He knows only advance,\nnot retreat. But this means exhaustion. In such a case it is important\nto be constantly mindful that one must be conscientious and consistent\nand must remain so. Only thus does one become free of blind impulse, which\nis always harmful.",
        }
      }
  },
  {
    .number = 47,
    .sk = 0b00011010,
    .name = "Oppression (Exhaustion)",
    .cm = "The lake is above, water below; the lake is empty, dried up. Exhaustion\nis expressed in yet another way: at the top, a dark line is holding down\ntwo light line; below, a light line is hemmed in between two dark ones.\nThe upper trigram belongs to the principle of darkness, the lower to the\nprinciple of light. Thus everywhere superior men are oppressed and held\nin restraint by inferior men.",
    .jd = "OPPRESSION. Success. Perseverance.\nThe great man brings about good fortune.\nNo blame.\nWhen one has something to say,\nIt is not believed.",
    .j_cm = "Times of adversity are the reverse of times of success, but they can\nlead to success if they; befall the right man. When a strong man meets\nwith adversity, he remains cheerful despite all danger, and this cheerfulness\nis the source of later successes; it is that stability which is stronger\nthan fate. He who lets his spirit be broken by exhaustion certainly has\nno success. But if adversity only bends a man, it creates in him a power\nto react that is bound in time to manifest itself. No inferior man is capable\nof this. Only the great man brings about goof fortune and remains blameless.\nIt is true that for the time being outward influence is denied him, because\nhis words have no effect. Therefore in times of adversity it is important\nto be strong within and sparing of words.",
    .im =   "There is not water in the lake:\nThe image of EXHAUSTION.\nThus the superior man stakes his life\nOn following his will.",
    .i_cm = "When the water has flowed out below, the lake must dry up and become\nexhausted. That is fate. This symbolizes an adverse fate in human life.\nIn such times there is nothing a man can do but acquiesce in his fate\nand remain true to himself. This concerns the deepest stratum of his being,\nfor this alone is superior to all external fate.",
        .lines = {
        {
          .pd ="One sits oppressed under a bare tree\nAnd strays into a gloomy valley.\nFor three years one sees nothing.",
          .cm = "When adversity befalls a man, it is important above all things for\nhim to be strong and to overcome the trouble inwardly. If he is weak,\nthe trouble overwhelms him. Instead of proceeding on his way, he remains\nsitting under a bare tree and falls ever more deeply into gloom and melancholy.\nThis makes the situation only more and more hopeless. Such an attitude\ncomes from an inner delusion that he must by all means overcome.",
        },
        {
          .pd ="One is oppressed while at meat and drink.\nThe man with the scarlet knee bands is just coming.\nIt furthers one to offer sacrifice.\nTo set forth brings misfortune.\nNo blame.",
          .cm = "This pictures a state of inner oppression. Externally, all is well,\none has meat and drink. But one is exhausted by the commonplaces of life,\nand there seems to be no way of escape. Then help comes from a high place.\nA prince-in ancient China princes wore scarlet knee bands- is in search\nof able helpers. But there are still obstructions to be overcome. Therefore\nit is important to meet these obstructions in the visible realm by offerings\nand prayer. To set forth without being prepared would be disastrous,\nthough not morally wrong. Here a disagreeable situation must be overcome\nby patience of spirit.",
        },
        {
          .pd ="A man permits himself to be oppressed by stone,\nAnd leans on thorns and thistles.\nHe enters the house and does not see his wife.\nMisfortune.",
          .cm = "This shows a man who is restless and indecisive in times of adversity.\nAt first he wants to push ahead, then he encounters obstructions that,\nit is true, mean oppression only when recklessly dealt with. He butts\nhis head against a wall and in consequence feels himself oppressed by\nthe wall. Then he leans on things that have in themselves no stability\nand that are merely a hazard for him who leans on them. Thereupon he\nturns back irresolutely and retires into his house, only to find, as\na fresh disappointment, that his wife is not there. Confucius says about\nthis line:\nIf a man permits himself to be oppressed by something that ought not\nto oppress him, his name will certainly be disgraced. If he leans on\nthings upon which one cannot lean, his life will certainly be endangered.\nFor him who is in disgrace and danger, the hour of death draws near; how\ncan he then still see his wife?",
        },
        {
          .pd ="He comes very quietly, oppressed in a golden carriage.\nHumiliation, but the end is reached.",
          .cm = "A well-to-do man sees the need of the lower classes and would like\nvery much to be of help. But instead of proceeding with speed and energy\nwhere their is need, he begins in a hesitant and measured way. Then he\nencounters obstructions. Powerful and wealthy acquaintances draw him into\ntheir circle; he has to do as they do and cannot withdraw from them. Hence\nhe finds himself in great embarrassment. But the trouble is transitory.\nThe original strength of his nature offsets the mistake he has made, and\nthe goal is reached.",
        },
        {
          .pd ="His nose and feet are cut off.\nOppression at the hands of the man with the purple knee bands.\nJoy comes softly.\nIt furthers one to make offerings and libations.",
          .cm = "An individual who has the good of mankind at heart is oppressed from\nabove and below (this is the meaning of the cutting off of nose an defeat).\nHe finds no help among the people whose duty it would be to aid in the\nwork of rescue (ministers wore purple knee bands). But little by little,\nthings take a turn for the better. Until that time, he should turn to\nGod, firm in his inner composure, and pray and offer sacrifice for the\ngeneral well-being.",
        },
        {
          .pd ="He is oppressed by creeping vines.\nHe moves uncertainly and says, \"Movement brings remorse.\"\nIf one feels remorse over this and makes a start,\nGood fortune comes.",
          .cm = "A man is oppressed by bonds that can easily be broken. The distress\nis drawing to an end. But he is still irresolute; he is still influenced\nby the previous condition and fears that he may have cause for regret\nif he makes a move. But as soon as he grasps the situation, changes this\nmental attitude, and makes a firm decision, he masters the oppression.",
        }
      }
  },
  {
    .number = 48,
    .sk = 0b00010110,
    .name = "The Well",
    .cm = "Wood is below, water above. The wood goes down into the earth to bring\nup water. The image derives from the pole-and-bucket well of ancient\nChina. The wood represents not the buckets, which in ancient times were\nmade of clay, but rather the wooden poles by which the water is hauled\nup from the well. The image also refers to the world of plants, which lift\nwater out of the earth by means of their fibers. The well from which water\nis drawn conveys the further idea of an inexhaustible dispensing of nourishment.",
    .jd = "THE WELL. The town may be changed,\nBut the well cannot be changed.\nIt neither decreases nor increases.\nThey come and go and draw from the well.\nIf one gets down almost to the water\nAnd the rope does not go all the way,\nOr the jug breaks, it brings misfortune.",
    .j_cm = "In ancient China the capital cities were sometimes moved, partly for\nthe sake of more favorable location, partly because of a change in dynasties.\nThe style of architecture changed in the course of centuries, but the\nshape of the well has remained the same from ancient times to this day.\nThus the well is the symbol of that social structure which, evolved by\nmankind in meeting its most primitive needs, is independent of all political\nforms. Political structures change, as do nations, but the life of man with\nits needs remains eternally the same-this cannot be changed. Life is also\ninexhaustible. It grows neither less not more; it exists for one and for\nall. The generations come and go, and all enjoy life in its inexhaustible\nabundance. However, there are two prerequisites for a satisfactory political\nor social organization of mankind. We must go down to the very foundations\nof life. For any merely superficial ordering of life that leaves its deepest\nneeds unsatisfied is as ineffectual as if no attempt at order had ever\nbeen made. Carelessness-by which the jug is broken-is also disastrous.\nIf for instance the military defense of a state is carried to such excess\nthat it provokes wars by which the power of the state is annihilated, this\nis a breaking of the jug. This hexagram applies also to the individual.\nHowever men may differ in disposition and in education, the foundations\nof human nature are the same in everyone. And every human being can draw\nin the course of his education from the inexhaustible wellspring of the\ndivine in man's nature. But here likewise two dangers threaten: a man may\nfail in his education to penetrate to the real roots of humanity and remain\nfixed in convention-a partial education of this sort is as bad as none-\nor he may suddenly collapse and neglect his self-development.",
    .im =   "Water over wood: \nthe image of THE WELL.\nThus the superior man encourages the people at their work,\nAnd exhorts them to help one another.",
    .i_cm = "The trigram Sun, wood, is below, and the trigram K'an, water, is above\nit. Wood sucks water upward. Just as wood as an organism imitates the\naction of the well, which benefits all parts of the plant, the superior\nman organizes human society, so that, as in a plant organism, its parts\nco-operate for the benefit of the whole.",
        .lines = {
        {
          .pd ="One does not drink the mud of the well.\nNo animals come to an old well.",
          .cm = "If a man wanders around in swampy lowlands, his life is submerged\nin mud. Such a man loses all significance for mankind. He who throws himself\naway is no longer sought out by others. In the end no one troubles about\nhim any more.",
        },
        {
          .pd ="At the well hole one shoots fishes.\nThe jug is broken and leaks.",
          .cm = "The water itself is clear, but it is not being used. Thus the well\nis a place where only fish will stay, and whoever comes to it, comes\nonly to catch fish. But the jug is broken, so that the fish cannot be\nkept in it. This describes the situation of a person who possesses good\nqualities but neglects them. No one bothers about him. As a result he\ndeteriorates in mind. He associates with inferior men and can no longer\naccomplish anything worth while.",
        },
        {
          .pd ="The well is cleaned, but no one drinks from it.\nThis is my heart's sorrow,\nFor one might draw from it.\nIf the king were clear-minded,\nGood fortune might be enjoyed in common.",
          .cm = "An able man is available. He is like a purified well whose water is\ndrinkable. But no use is made of him. This is the sorrow of those who\nknow him. One wishes that the prince might learn about it; this would\nbe good fortune for all concerned.",
        },
        {
          .pd ="The well is being lined. No blame.",
          .cm = "True, if a well is being lined with sone, it cannot be used while\nthe work is going on. But the work is not in vain; the result is that the\nwater stays clear. In life also there are times when a man must put himself\nin order. During such a time he can do nothing for others, but his work is\nnonetheless valuable, because by enhancing his powers and abilities through\ninner development, he can accomplish all the more later on.",
        },
        {
          .pd ="In the well there is a clear, cold spring\nFrom which one can drink.",
          .cm = "A well that is fed by a spring of living water is a good well. A man\nwho has virtues like a well of this sort is born to be a leader and savior\nof men, for he has the water of life. Nevertheless, the character for\n\"good fortune\" is left out here. The all-important thing about a well\nis that its water be drawn. The best water is only a potentiality for refreshment\nas long as it is not brought up. So too with leaders of mankind: it is\nall-important that one should drink from the spring of their words and\ntranslate them into life.",
        },
        {
          .pd ="One draws from the well\nWithout hindrance.\nIt is dependable.\nSupreme good fortune.",
          .cm = "The well is there fore all. No one is forbidden to take water from\nit. No matter how many come, all find what they need, for the well is\ndependable. It has a spring and never runs dry. Therefore it is a great\nblessing to the whole land. The same is true of the really great man, whose\ninner wealth is inexhaustible; the more that people draw from him, the\ngreater his wealth becomes.",
        }
      }
  },
  {
    .number = 49,
    .sk = 0b00011101,
    .name = "Revolution (Molting)",
    .cm = "The Chinese character for this hexagram means in its original sense\nan animal's pelt, which is changed in the course of the year by molting.\nFrom this word is carried over to apply to the \"moltings\" in political\nlife, the great revolutions connected with changes of governments. The\ntwo trigrams making up the hexagram are the same two that appear in K'uei,\nOPPOSITION 38, that is, the two younger daughters, Li and Tui.\nBut while there the elder of the two daughters is above, and what results\nis essentially only an opposition of tendencies, here the younger daughter\nis above. The influences are in actual conflict, and the forces combat\neach other like fire and water (lake), each trying to destroy the other.\nHence the idea of revolution.",
    .jd = "REVOLUTION. On your own day\nYou are believed.\nSupreme success,\nFurthering through perseverance.\nRemorse disappears.",
    .j_cm = "Political revolutions are extremely grave matters. They should be\nundertaken only under stress of direst necessity, when there is no other\nway out. Not everyone is called to this task, but only the man who has the\nconfidence of the people, and even he only when the time is ripe. He must\nthen proceed in the right way, so that he gladdens the people and,\nby enlightening them, prevents excesses.\nFurthermore, he must be quite free of selfish aims\nand must really relieve the need of the people. Only then does he have nothing\nto regret. Times change, and with them their demands. Thus the seasons\nchange in the course of the year. In the world cycle also there are spring\nand autumn in the life of peoples and nations, and these call for social\ntransformations.",
    .im =   "Fire in the lake: \nthe image of REVOLUTION.\nThus the superior man\nSets the calendar in order\nAnd makes the seasons clear.",
    .i_cm = "Fire below and the lake above combat and destroy each other. So too\nin the course of the year a combat takes place between the forces of\nlight and the forces of darkness, eventuating in the revolution of the\nseasons, and man is able to adjust himself in advance to the demands\nof the different times.",
        .lines = {
        {
          .pd ="Wrapped in the hide of a yellow cow.",
          .cm = "Changes ought to be undertaken only when there is nothing else to\nbe done. Therefore at first the utmost restraint is necessary. One must becomes\nfirm in one's mind, control oneself-yellow is the color of the means,\nand the cow is the symbol of docility-and refrain from doing anything\nfor the time being, because any premature offensive will bring evil results.",
        },
        {
          .pd ="When one's own day comes, one may create revolution.\nStarting brings good fortune. No blame.",
          .cm = "When we have tried in every other way to bring about reforms, but\nwithout success, revolution becomes necessary. But such a thoroughgoing upheaval\nmust be carefully prepared. There must be available a man who has the requisite\nabilities and who possesses public confidence. To such a man we may well\nturn. This brings good fortune and is not a mistake. The first thing to\nbe considered is our inner attitude toward the new condition that will\ninevitably come. We have to go out to meet it, as it were. Only in this\nway can it be prepared for.",
        },
        {
          .pd ="Starting brings misfortune.\nPerseverance brings danger.\nWhen talk of revolution has gone the rounds three times,\nOne may commit himself,\nAnd men will believe him.",
          .cm = "When change is necessary, there are two mistakes to be avoided. One\nlies in excessive haste and ruthlessness, which bring disaster. The other\nlies in excessive hesitation and conservatism, which are also dangerous.\nNot every demand for change in the existing order should be heeded. On\nthe other hand, repeated and well-founded complaints should not fail of\na hearing. When talk of change has come to one's ears three times, and\nhas been pondered well, he may believe and acquiesce in it. Then he will\nmeet with belief and will accomplish something.",
        },
        {
          .pd ="Remorse disappears. Men believe him.\nChanging the form of government brings good fortune.",
          .cm = "Radical changes require adequate authority. A man must have inner\nstrength as well as influential position. What he does must correspond with\na higher truth and must not spring from arbitrary or petty motives; then\nit brings great good fortune. If a revolution is not founded on such inner\ntruth, the results are bad, and it has no success. For in the end men will\nsupport only those undertakings which they feel instinctively to be just.",
        },
        {
          .pd ="The great man changes like a tiger.\nEven before he questions the oracle\nHe is believed.",
          .cm = "A tigerskin, with its highly visible black stripes on a yellow ground,\nshows its distinct pattern from afar. It is the same with a revolution\nbrought about by a great man: large, clear guiding lines become visible,\nunderstandable to everyone. Therefore he need not first consult the oracle,\nfor he wins the spontaneous support of the people.",
        },
        {
          .pd ="The superior man changes like a panther.\nThe inferior man molts in the face.\nStarting brings misfortune.\nTo remain persevering brings good fortune.",
          .cm = "After the large and fundamental problems are settled, certain minor\nreforms, and elaborations of these, are necessary. These detailed reforms\nmay be likened to the equally distinct but relatively small marks of\nthe panther's coat. As a consequence, a change also takes place among\nthe inferior people. In conformity with the new order, they likewise \"molt.\"\nThis molting, it is true, does not go very deep, but that is not to be\nexpected. We must be satisfied with the attainable. If we should go too\nfar and try to achieve too much, it would lead to unrest and misfortune.\nFor the object of a great revolution is the attainment of clarified, secure\nconditions ensuring a general stabilization on the basis of what is possible\nat the moment.",
        }
      }
  },
  {
    .number = 50,
    .sk = 0b00101110,
    .name = "The Caldron",
    .cm = "The six lines construct the image of Ting, THE CALDRON; at the bottom\nare the legs, over them the belly, then come the ears (handles), and\nat the top the carrying rings. At the same time, the image suggests the\nidea of nourishment. The ting, cast of bronze, was the vessel that held\nthe cooked viands in the temple of the ancestors and at banquets. The\nheads of the family served the food from the ting into the bowls of the\nguests. THE WELL 48 likewise has the secondary meaning of giving\nnourishment, but rather more in relation to the people. The ting, as a utensil\npertaining to a refined civilization, suggests the fostering and nourishing\nof able men, which redounded to the benefit of the state. This hexagram\nand THE WELL are the only two in the Book of Changes that represent concrete,\nmen-made objects. Yet here too the thought has its abstract connotation.\nSun, below, is wood and wind; Li, above, is flame. Thus together they stand\nfor the flame kindled by wood and wind, which likewise suggests the idea\nof preparing food.",
    .jd = "THE CALDRON. Supreme good fortune.\nSuccess.",
    .j_cm = "While THE WELL relates to the social foundation of our life, and this\nfoundation is likened to the water that serves to nourish growing wood,\nthe present hexagram refers to the cultural superstructure of society.\nHere it is the wood that serves as nourishment for the flame, the spirit.\nAll that is visible must grow beyond itself, extend into the realm of the\ninvisible. Thereby it receives its true consecration and clarity and takes\nfirm root in the cosmic order. Here we see civilization as it reaches\nits culmination in religion. The ting serves in offering sacrifice to\nGod. The highest earthly values must be sacrificed to the divine. But the\ntruly divine does not manifest itself apart from man. The supreme revelation\nof God appears in prophets and holy men. To venerate them is true veneration\nof God. The will of God, as revealed through them, should be accepted in\nhumility; this brings inner enlightenment and true understanding of the\nworld, and this leads to great good fortune and success.",
    .im =   "Fire over wood:\nThe image of THE CALDRON.\nThus the superior man consolidates his fate\nBy making his position correct.",
    .i_cm = "The fate of fire depends on wood; as long as there is wood below,\nthe fire burns above. It is the same in human life; there is in man likewise\na fate that lends power to his life. And if he succeeds in assigning the\nright place to life and to fate, thus bringing the two into harmony, he puts\nhis fate on a firm footing. These words contain hints about fostering of\nlife as handed on by oral tradition in the secret teachings of Chinese yoga.",
        .lines = {
        {
          .pd ="A ting with legs upturned.\nFurthers removal of stagnating stuff.\nOne takes a concubine for the sake of her son.\nNo blame.",
          .cm = "If a ting is turned upside down before being used, no harm is done-on\nthe contrary, this clears it of refuse. A concubine's position is lowly,\nbut because she has a son she comes to be honored. These two metaphors\nexpress the idea that in a highly developed civilization, such as that\nindicated by this hexagram, every person of good will can in some way or\nother succeed. No matter how lowly he may be, provided he is ready to purify\nhimself, he is accepted. He attains a station in which he can prove himself\nfruitful in accomplishment, and as a result he gains recognition.",
        },
        {
          .pd ="There is food in the ting.\nMy comrades are envious,\nBut they cannot harm me.\nGood fortune.",
          .cm = "In a period of advanced culture, it is of the greatest importance\nthat one should achieve something significant. If a man concentrates on such\nreal undertakings, he may indeed experience envy and disfavor, but that is\nnot dangerous. The more he limits himself to his actual achievements, the\nless harm the envious inflict on him.",
        },
        {
          .pd ="The handle of the ting is altered.\nOne is impeded in his way of life.\nThe fat of the pheasant is not eaten.\nOnce rain falls, remorse is spent.\nGood fortune comes in the end.",
          .cm = "The handle is the means for lifting up the ting. If the handle is\naltered, the ting cannot be lifted up and used, and, sad to say, the delicious\nfood in it, such as pheasant fat, cannot be eaten by anyone. This describes\na man who, in a highly evolved civilization, finds himself in a place where\nno one notices or recognizes him. This is a severe block to his effectiveness.\nAll of his good qualities and gifts of mind thus needlessly go to waste.\nBut if he will only see to it that he is possessed of something truly\nspiritual, the time is bound to come, sooner or later, when the difficulties\nwill be resolved and all will go well. The fall of rain symbolizes here,\nas in other instances, release of tension.",
        },
        {
          .pd ="The legs of the ting are broken.\nThe prince's meal is spilled\nAnd his person is soiled.\nMisfortune.",
          .cm = "A man has a difficult and responsible task to which he is not adequate.\nMoreover, he does not devote himself to it with all his strength but\ngoes about with inferior people; therefore the execution of the work\nfails. In this way he also incurs personal opprobrium. Confucius says\nabout this line: \"Weak character coupled with honored place, meager knowledge\nwith large plans, limited powers with heavy responsibility, will seldom\nescape disaster.\"",
        },
        {
          .pd ="The ting has yellow handles, golden carrying rings.\nPerseverance furthers.",
          .cm = "Here we have, in a ruling position, a man who is approachable and\nmodest in nature. As a result of this attitude he succeeds in finding strong\nand able helpers who complement and aid him in his work. Having achieved\nthis attitude, which requires constant self-abnegation, it is important for\nhim to hold to it and not to let himself be led astray.",
        },
        {
          .pd ="The ting has rings of jade.\nGreat good fortune.\nNothing that would not act to further.",
          .cm = "In the preceding line the carrying rings are described as golden,\nto denote their strength; here they are said to be of jade. Jade is notable\nfor its combination of hardness with soft luster. This counsel, in relation\nto the man who is open to it, works greatly t his advantage. Here the counsel\nis described in relation to the sage who imparts it. In imparting it,\nhe will be mild and pure, like precious jade. Thus the work finds favor\nin the eyes of the Deity, who dispenses great good fortune, and becomes\npleasing to men, wherefore all goes well.",
        }
      }
  },
  {
    .number = 51,
    .sk = 0b00001001,
    .name = "The Arousing (Shock, Thunder)",
    .cm = "The hexagram Chên represents the eldest son, who seizes rule\nwith energy and power. A yang line develops below two yin lines and presses\nupward forcibly. This movement is so violent that it arouses terror.\nIt is symbolized by thunder, which bursts forth from the earth and by\nits shock causes fear and trembling.",
    .jd = "SHOCK brings success.\nShock comes-oh, oh!\nLaughing words -ha, ha!\nThe shock terrifies for a hundred miles,\nAnd he does not let fall the sacrificial spoon and chalice.",
    .j_cm = "The shock that comes from the manifestation of God within the depths\nof the earth makes man afraid, but this fear of God is good, for joy\nand merriment can follow upon it. When a man has learned within his heart\nwhat fear and trembling mean, he is safeguarded against any terror produced\nby outside influences. Let the thunder roll and spread terror a hundred\nmiles around: he remains so composed and reverent in spirit that the sacrificial\nrite is not interrupted. This is the spirit that must animate leaders\nand rulers of men-a profound inner seriousness from which all terrors\nglance off harmlessly.",
    .im =   "Thunder repeated: \nThe image of SHOCK.\nThus in fear and trembling\nThe superior man sets his life in order\nAnd examines himself.",
    .i_cm = "The shock of continuing thunder brings fear and trembling. The superior\nman is always filled with reverence at the manifestation of God; he sets\nhis life in order and searches his heart, lest it harbor any secret opposition\nto the will of God. Thus reverence is the foundation of true culture.",
        .lines = {
        {
          .pd ="Shock comes-oh, oh!\nThen follow laughing words-ha, ha!\nGood fortune.",
          .cm = "The fear and trembling engendered by shock come to an individual at\nfirst in such a way that he sees himself placed at a disadvantage as\nagainst others. But this is only transitory. When the ordeal is over,\nhe experiences relief, and thus the very terror he had to endure at the\noutset brings good fortune in the long run.",
        },
        {
          .pd ="Shock comes bringing danger.\nA hundred thousand times\nYou lose your treasures\nAnd must climb the nine hills.\nDo not go in pursuit of them.\nAfter seven days you will get them back again.",
          .cm = "This pictures a situation in which a shock endangers a man and he\nsuffers great losses. Resistance would be contrary to the movement of the\ntime and for this reason unsuccessful. Therefore he must simply retreat to\nheights inaccessible to the threatening forces of danger. He must accept\nhis loss of property without worrying too much about it. When the time of\nshock and upheaval that has robbed him of his possessions has passed, he\nwill get them back again without going in pursuit of them.",
        },
        {
          .pd ="Shock comes and makes one distraught.\nIf shock spurs to action\nOne remains free of misfortune.",
          .cm = "There are three kinds of shock-the shock of heaven, which is thunder,\nthe shock of fate, and, finally, the shock of the heart. The present\nhexagram refers less to inner shock than to the shock of fate. In such\ntimes of shock, presence of mind is all too easily lost: the individual\noverlooks all opportunities for action and mutely lets fate take its course.\nBut if he allows the shocks of fate to induce movement within his mind,\nhe will overcome these external blows with little effort.",
        },
        {
          .pd ="Shock is mired.",
          .cm = "Movement within the mind depends for its success partly on circumstances.\nIf there is neither a resistance that might be vigorously combated, nor\nyet a yielding that permits of victory-if, instead, everything is tough\nand inert like mire-movement is crippled.",
        },
        {
          .pd ="Shock goes hither and thither.\nDanger.\nHowever, nothing at all is lost.\nYet there are things to be done.",
          .cm = "This is a case not of a single shock but of repeated shocks with no\nbreathing space between. Nonetheless, the shock causes no loss, because\none takes care to stay in the center of movement and in this way to be\nspared the fate of being helplessly tossed hither and thither.",
        },
        {
          .pd ="Shock brings ruin and terrified gazing around.\nGoing ahead brings misfortune.\nIf it has not yet touched one's own body\nBut has reached one's neighbor first,\nThere is no blame.\nOne's comrades have something to talk about.",
          .cm = "When inner shock is at its height, it robs a man of reflection and\nclarity of vision. In such a state of shock it is of course impossible\nto act with presence of mind. Then the right thing is to keep still until\ncomposure and clarity are restored. But this a man can do only when he\nhimself is not yet infected by the agitation, although its disastrous effects\nare already visible in those around him. If he withdraws from the affair\nin time, he remains free of mistakes and injury. But his comrades, who no\nlonger heed any warning, will in their excitement certainly be displeased\nwith him. However, he must not take this into account.",
        }
      }
  },
  {
    .number = 52,
    .sk = 0b00100100,
    .name = "Keeping Still, Mountain",
    .cm = "The image of this hexagram is the mountain, the youngest son of heaven\nand earth. The male principle is at the top because it strives upward\nby nature; the female principle is below, since the direction of its movement\nhas come to its normal end. In its application to man, the hexagram turns\nupon the problem of achieving a quiet heart. It is very difficult to bring\nquiet to the heart. While Buddhism strives for rest through an ebbing\naway of all movement in nirvana, the Book of Changes holds that rest is\nmerely a state of polarity that always posits movement as its complement.\nPossibly the words of the text embody directions for the practice of yoga.",
    .jd = "KEEPING STILL. Keeping his back still\nSo that he no longer feels his body.\nHe goes into his courtyard\nAnd does not see his people.\nNo blame.",
    .j_cm = "True quiet means keeping still when the time has come to keep still,\nand going forward when the time has come to go forward. In this way rest\nand movement are in agreement with the demands of the time, and thus there\nis light in life. The hexagram signifies the end and the beginning of\nall movement. The back is named because in the back are located all the\nnerve fibers that mediate movement. If the movement of these spinal nerves\nis brought to a standstill, the ego, with its restlessness, disappears as\nit were. When a man has thus become calm, he may turn to the outside world.\nHe no longer sees in it the struggle and tumult of individual beings, and\ntherefore he has that true peace of mind which is needed for understanding\nthe great laws of the universe and for acting in harmony with them. Whoever\nacts from these deep levels makes no mistakes.",
    .im =   "Mountains standing close together:\nThe image of KEEPING STILL.\nThus the superior man\nDoes not permit his thoughts\nTo go beyond his situation.",
    .i_cm = "The heart thinks constantly. This cannot be changed, but the movements\nof the heart-that is, a man's thoughts-should restrict themselves to\nthe immediate situation. All thinking that goes beyond this only makes\nthe heart sore.",
        .lines = {
        {
          .pd ="Keeping his toes still.\nNo blame.\nContinued perseverance furthers.",
          .cm = "Keeping the toes still means halting before one has even begun to\nmove. The beginning is the time of few mistakes. At that time one is still\nin harmony with primal innocence. Not yet influenced by obscuring interests\nand desires, one sees things intuitively as they really are. A man who halts\nat the beginning, so long as he has not yet abandoned the truth, finds the\nright way. But persisting firmness is needed to keep one from drifting irresolutely.",
        },
        {
          .pd ="Keeping his calves still.\nHe cannot rescue him whom he follows.\nHis heart is not glad.",
          .cm = "The leg cannot move independently; it depends on the movement of the\nbody. If a leg is suddenly stopped while the whole body is in vigorous\nmotion, the continuing body movement will make one fall. The same is\ntrue of a man who serves a master stronger than himself. He is swept along,\nand even though he may himself halt on the path of wrongdoing, he can no\nlonger check the other in his powerful movement. Where the master presses\nforward, the servant, no matter how good his intentions, cannot save him.",
        },
        {
          .pd ="Keeping his hips still.\nMaking his sacrum stiff.\nDangerous. The heart suffocates.",
          .cm = "This refers to enforced quiet. The restless heart is to be subdued\nby forcible means. But fire when it is smothered changes into acrid smoke\nthat suffocates as it spreads. Therefore, in exercises in meditation\nand concentration, one ought not to try to force results. Rather, calmness\nmust develop naturally out of a state of inner composure. If one tries\nto induce calmness by means of artificial rigidity, meditation will lead\nto very unwholesome results.",
        },
        {
          .pd ="Keeping his trunk still.\nNo blame.",
          .cm = "As has been pointed out above in the comment on the Judgment, keeping\nthe back at rest means forgetting the ego. This is the highest stage\nof rest. Here this stage has not yet been reached: the individual in this\ninstance, though able to keep the ego, with its thoughts and impulses,\nin a state of rest, is not yet quite liberated from its dominance. Nonetheless,\nkeeping the heart at rest is an important function, leading in the end\nto the complete elimination of egotistic drives. Even though at this point\none does not yet remain free from all the dangers of doubt and unrest,\nthis frame of mind is not a mistake, as it leads ultimately to that other,\nhigher level.",
        },
        {
          .pd ="Keeping his jaws still.\nThe words have order.\nRemorse disappears.",
          .cm = "A man in a dangerous situation, especially when he is not adequate\nto it, is inclined to be very free with talk and presumptuous jokes.\nBut injudicious speech easily leads to situations that subsequently give\nmuch cause for regret. However, if a man is reserved in speech, his words\ntake ever more definite form, and every occasion for regret vanishes.",
        },
        {
          .pd ="Noblehearted keeping still.\nGood fortune.",
          .cm = "This marks the consummation of the effort to attain tranquillity.\nOne is at rest, not merely in a small, circumscribed way in regard to matters\nof detail, but one has also a general resignation in regard to life as\na whole, and this confers peace and good fortune in relation to every individual\nmatter.",
        }
      }
  },
  {
    .number = 53,
    .sk = 0b00110100,
    .name = "Development (Gradual Progress)",
    .cm = "This hexagram is made up of Sun (wood, penetration) above, i.e., without,\nand Kên (mountain, stillness) below, i.e., within. A tree on a\nmountain develops slowly according to the law of its being and consequently\nstands firmly rooted. This gives the idea of a development that proceeds\ngradually, step by step. The attributes of the trigrams also point to\nthis: within is tranquillity, which guards against precipitate actions,\nand without is penetration, which makes development and progress possible.",
    .jd = "DEVELOPMENT. The maiden\nIs given in marriage.\nGood fortune.\nPerseverance furthers.",
    .j_cm = "The development of events that leads to a girl's following a man to\nhis home proceeds slowly. The various formalities must be disposed of\nbefore the marriage takes place. This principle of gradual development\ncan be applied to other situations as well; it is always applicable where\nit is a matter of correct relationships of co-operation, as for instance\nin the appointment of an official. The development must be allowed to take\nits proper course. Hasty action would not be wise. This is also true, finally,\nof any effort to exert influence on others, for here too the essential\nfactor is a correct way of development through cultivation of one's own\npersonality. No influence such as that exerted by agitators has a lasting\neffect. Within the personality too, development must follow the same course\nif lasting results are to be achieved. Gentleness that is adaptable, but\nat the same time penetrating, is the outer form that should proceed from\ninner calm. The very gradualness of the development makes it necessary\nto have perseverance, for perseverance alone prevents slow progress from\ndwindling to nothing.",
    .im =   "On the mountain, a tree:\nThe image of DEVELOPMENT.\nThus the superior man abides in dignity and virtue,\nIn order to improve the mores.",
    .i_cm = "The tree on the mountain is visible from afar, and its development\ninfluences the landscape of the entire region. It does not shoot up like\na swamp plant; its growth proceeds gradually. Thus also the work of influencing\npeople can be only gradual. No sudden influence or awakening is of lasting\neffect. Progress must be quite gradual, and in order to obtain such progress\nin public opinion and in the mores of the people, it is necessary for\nthe personality to acquire influence and weight. This comes about through\ncareful and constant work on one's own moral development.",
        .lines = {
        {
          .pd ="The wild goose gradually draws near the shore.\nThe young son is in danger.\nThere is talk. No blame.",
          .cm = "All the individual lines in this hexagram symbolize the gradual flight\nof the wild goose. The wild goose is the symbol of conjugal fidelity,\nbecause it is believed that this bird never takes another mate after the\ndeath of the first. The initial line suggests the first resting place\nin the flight of water birds from the water to the heights. The shore is\nreached. The situation is that of a lonely young man who is just starting\nout to make his way in life. Since no one comes to help him, his first steps\nare slow and hesitant, and he is surrounded by danger. Naturally he is subjected\nto much criticism. But these very difficulties keep him from being too\nhasty, and his progress is successful.",
        },
        {
          .pd ="The wild goose gradually draws near the cliff.\nEating and drinking in peace and concord.\nGood fortune.",
          .cm = "The cliff is a safe place on shore. The development has gone a step\nfurther. The initial insecurity has been overcome, and a safe position\nin life has been found, giving one enough to live on. This first success,\nopening up a path to activity, brings a certain joyousness of mood, and\none goes to meet the future reassured. It is said of the wild goose that\nit calls to its comrades whenever it finds food; this is the symbol of\npeace and concord in good fortune. A man does not want to keep his good\nluck for himself only, but is ready to share it with others.",
        },
        {
          .pd ="The wild goose gradually draws near the plateau.\nThe man goes forth and does not return.\nThe woman carries a child but does not bring it forth.\nMisfortune.\nIt furthers one to fight off robbers.",
          .cm = "The high plateau is dry and unsuitable for the wild goose. If it goes\nthere, it has lost its way and gone too far. This is contrary to the\nlaw of development. It is the same in human life. If we do not let things\ndevelop quietly but plunge of our own choice too rashly into a struggle,\nmisfortune results. A man jeopardizes his own life, and his family perishes\nthereby. However, this is not all necessary; it is only the result of\ntransgressing the law of natural development. If one does not willfully\nprovoke a conflict, but confines himself to vigorously maintaining his\nown position and to warding off unjustified attacks, all goes well.",
        },
        {
          .pd ="The wild goose goes gradually draws near the tree.\nPerhaps it will find a flat branch. No blame.",
          .cm = "A tree is not a suitable place for a wild goose. But if it is clever,\nit will find a flat branch on which it can get a footing. A man's life\ntoo, in the course of its development, often brings him into inappropriate\nsituations, in which he finds it difficult to hold his own without danger.\nThen it is important to be sensible and yielding. This enables him to\ndiscover a safe place in which life can go on, although he may be surrounded\nby danger.",
        },
        {
          .pd ="The wild goose gradually draws near the summit.\nFor three years the woman has no child.\nIn the end nothing can hinder her.\nGood fortune.",
          .cm = "The summit is a high place. In a high position one easily becomes\nisolated. One is misjudged by the very person on whom one is dependent-the\nwoman by her husband, the official by his superior. This is the work of deceitful\npersons who have wormed their way in. The result is that relationships remain\nsterile, and nothing is accomplished. But in the course of further development,\nsuch misunderstandings are cleared away, and reconciliation is achieved after\nall.",
        },
        {
          .pd ="The wild goose gradually draws near the clouds heights.\nIts feathers can be used for the sacred dance.\nGood fortune.",
          .cm = "Here life comes to its end. A man's work stands completed. The path\nrises high toward heaven, like the flight of wild geese when they have\nleft the earth far behind. There they fly, keeping to the order of their\nflight in strict formation. And if their feathers fall, they can serve\nas ornaments in the sacred dance pantomimes performed in the temples. Thus\nthe life of a man who has perfected himself is a bright light for the\npeople of earth, who look up to him as an example.",
        }
      }
  },
  {
    .number = 54,
    .sk = 0b00001011,
    .name = "The Marrying Maiden",
    .cm = "Above we have Chên, the eldest son, and below, Tui, the youngest\ndaughter. The man leads and the girl follows him in gladness. The picture\nis that of the entrance of the girl into her husband's house. In all,\nthere are four hexagrams depicting the relationship between husband and\nwife. Hsien, INFLUENCE 31, describes the attraction that a young couple\nhave for each other;\nHêng, DURATION 32, portrays the permanent relationships of marriage;\nChien, DEVELOPMENT 53 reflects the protracted, ceremonious procedures\nattending THE MARRYING MAIDEN, shows a young girl under the guidance\nof an older man who marries her.",
    .jd = "THE MARRYING MAIDEN.\nUndertakings bring misfortune.\nNothing that would further.",
    .j_cm = "A girl who is taken into the family, but not as the chief wife, must\nbehave with special caution and reserve. She must not take it upon herself\nto supplant the mistress of the house, for that would mean disorder and\nlead to untenable relationships. The same is true of all voluntary relationships\nbetween human beings. While legally regulated relationships based on\npersonal inclination depend in the long run entirely on tactful reserve.\nAffection as the essential principle of relatedness is of the greatest\nimportance in all relationships in the world. For the union of heaven\nand earth is the origin of the whole of nature. Among human beings likewise,\nspontaneous affection is the all-inclusive principle of union.",
    .im =   "Thunder over the lake:\nThe image of THE MARRYING MAIDEN.\nThus the superior man\nUnderstands the transitory\nIn the light of the eternity of the end.",
    .i_cm = "Thunder stirs the water of the lake, which follows it in shimmering\nwaves. This symbolizes the girl who follows the man of her choice. But\nevery relationship between individuals bears within it the danger that\nwrong turns may be taken,\nleading to endless misunderstandings and disagreements.\n\nTherefore it is necessary constantly to remain mindful of the end. If\nwe permit ourselves to drift along, we come together and are parted again\nas the day may determine. If on the other hand a man fixes his mind on\nan end that endures, he will succeed in avoiding the reefs that confront\nthe closer relationships of people.",
        .lines = {
        {
          .pd ="The marrying maiden as a concubine.\nA lame man who is able to tread.\nUndertakings bring good fortune.",
          .cm = "The princess of ancient China maintained a fixed order of rank among\nthe court ladies, who were subordinated to the queen as are younger sisters\nto the eldest. Frequently they came from the family of the queen, who\nherself led them to her husband. The meaning is that a girl entering\na family with the consent of the wife will not rank outwardly as the equal\nof the latter but will withdraw modestly into the background. However,\nif she understands how to fit herself into the pattern of things, her position\nwill be entirely satisfactory, and she will feel sheltered in the love\nof the husband to whom she bears children. The same meaning is brought\nout in the relationships between officials. A man may enjoy the personal\nfriendship of a prince and be taken into his confidence. Outwardly this\nman must keep tactfully in the background behind the official ministers\nof state, but, although he is hampered by this status, as if he were lame,\nhe can nevertheless accomplish something through the kindliness of his nature.",
        },
        {
          .pd ="A one-eyed man who is able to see.\nThe perseverance of a solitary man furthers.",
          .cm = "Here the situation is that of a girl married to a man who has disappointed\nher. Man and wife ought to work together like a pair of eyes. Here the\ngirl is left behind in loneliness; the man of her choice either has become\nunfaithful or has died. But she does not lost the inner light of loyalty.\nThought the other eye is gone, she maintains her loyalty even in loneliness.",
        },
        {
          .pd ="The marrying maiden as a slave.\nShe marries as a concubine.",
          .cm = "A girl who is in a lowly position and finds no husband may, in some\ncircumstances, still win shelter as a concubine. This pictures the situation\nof a person who longs too much for joys that cannot be obtained in the\nusual way. He enters upon a situation not altogether compatible with self-esteem.\nNeither judgment nor warning is added to this line; it merely lays bare\nthe actual situation, so that everyone may draw a lesson from it.",
        },
        {
          .pd ="The marrying maiden draws out the allotted time.\nA late marriage comes in due course.",
          .cm = "The girl is virtuous. She does not wish to throw herself away, and\nallows the customary time for marriage to slip by. However, there is\nno harm in this; she is rewarded for her purity and, even though belatedly,\nfinds the husband intended for her.",
        },
        {
          .pd ="The sovereign I gave his daughter in marriage.\nThe embroidered garments of the princess\nWere not as gorgeous\nAs those of the serving maid.\nThe moon that is nearly full\nBrings good fortune.",
          .cm = "The sovereign I is T'ang the Completer. This ruler decreed that the\nimperial princesses should be subordinated to their husbands in the same\nmanner as other women (cf. Hexagram 11, six in the fifth place). The emperor does not\nwait for a suitor to woo his daughter but gives her in marriage when\nhe sees fit. Therefore it is in accord with custom for the girl's family\nto take the initiative here. We see here a girl of aristocratic birth\nwho marries a man of modest circumstances and understands how to adapt\nherself with grace to the new situation. She is free of all vanity of outer\nadornment, and forgetting her rank in her marriage, takes a place below\nthat of her husband, just as the moon, before it is quite full, does not\ndirectly face the sun.",
        },
        {
          .pd ="The woman holds the basket, but there are no fruits in it.\nThe man stabs the sheep, but no blood flows.\nNothing that acts to further.",
          .cm = "At the sacrifice to the ancestors, the woman had to present harvest\nofferings in a basket, while the man slaughtered the sacrificial animal\nwith his own hand. Here the ritual is only superficially fulfilled; the\nwoman takes an empty basket and the man stabs a sheep slaughtered beforehand-solely\nto preserve the forms. This impious, irreverent attitude bodes no good\nfor a marriage.",
        }
      }
  },
  {
    .number = 55,
    .sk = 0b00001101,
    .name = "Abundance [Fullness]",
    .cm = "Chên is movement; Li is flame, whose attribute is clarity. Clarity\nwithin, movement without-this produces greatness and abundance. The hexagram\npictures a period of advanced civilization. However, the fact that development\nhas reached a peak suggests that this extraordinary condition of abundance\ncannot be maintained permanently.",
    .jd = "ABUNDANCE has success.\nThe king attains abundance.\nBe not sad.\nBe like the sun at midday.",
    .j_cm = "It is not given to every mortal to bring about a time of outstanding\ngreatness and abundance. Only a born ruler of men is able to do it, because\nhis will is directed to what is great. Such a time of abundance is usually\nbrief. Therefore a sage might well feel sad in view of the decline that\nmust follow. But such sadness foes not befit him. Only a man who is inwardly\nfree of sorrow and care can lead in a time of abundance. He must be like\nthe sun at midday, illuminating and gladdening everything under heaven.",
    .im =   "Both thunder and lightning come:\nThe image of ABUNDANCE.\nThus the superior man decides lawsuits\nAnd carries out punishments.",
    .i_cm = "This hexagram has a certain connection with Shih Ho, BITING THROUGH\n21, in which thunder and lightning similarly appear\ntogether, but in the reverse order. In BITING THROUGH, laws are laid\ndown; here they are applied and enforced. Clarity [Li] within makes it\npossible to investigate the facts exactly, and shock [Chên] without\nensures a strict and precise carrying out of punishments.",
        .lines = {
        {
          .pd ="When a man meets his destined ruler,\nThey can be together ten days,\nAnd it is not a mistake.\nGoing meets with recognition.",
          .cm = "To bring about a time of abundance, a union of clarity with energetic\nmovement is needed. Two individuals possessed of these two attributes\nare suited to each other, and even if they spend an entire cycle of time\ntogether during the period of abundance, it will not be too long, nor\nis it a mistake. Therefore one may go forth, in order to make one's influence\nfelt; it will meet with recognition.",
        },
        {
          .pd ="The curtain is of such fullness\nThat the polestars can be seen at noon.\nThrough going one meets with mistrust and hate.\nIf one rouses him through truth,\nGood fortune comes.",
          .cm = "It often happens that plots and party intrigues, which have the darkening\neffect of an eclipse of the sun, come between a ruler intent on great\nachievement and the man who could effect great undertakings. Then, instead\nof the sun, we see the northern stars in the sky. The ruler is overshadowed\nby a party that has usurped power. If a man at such a time were to try\nto take energetic measures, he would encounter only mistrust and envy,\nwhich would prohibit all movement. The essential thing then is to hold\ninwardly to the power of truth, which in the end is so strong that it exerts\nan invisible influence on the ruler, so that all goes well.",
        },
        {
          .pd ="The underbrush is of such abundance\nThat the small stars can be seen at noon.\nHe breaks his right arm . No blame.",
          .cm = "The image is that of a progressive covering over of the sun. Here\nthe eclipse reaches totality, therefore even the small stars can be seen\nat noon. In the sphere of social relationships, this means that the prince\nis now so eclipsed that even the most insignificant persons can push themselves\ninto the foreground. This makes it impossible for an able man, though\nhe might be the right hand of the ruler, to undertake anything. It is as\nthough his arm were broken, but he is not to blame for being thus hindered\nin action.",
        },
        {
          .pd ="The curtain is of such fullness\nThat the polestars can be seen at noon.\nHe meets his ruler, who is of like kind.\nGood fortune.",
          .cm = "Here the darkness is already decreasing, therefore interrelated elements\ncome together. Here too the complement must be found-the necessary wisdom\nto complement joy of action. Then everything will go well. The complementary\nfactor postulated here is the reverse of the one in the first line. In\nthe latter, wisdom is to be complemented by energy, while here energy\nis complemented by wisdom.",
        },
        {
          .pd ="Lines are coming,\nBlessing and fame draw near.\nGood fortune.",
          .cm = "The ruler is modest and therefore open to the counsel of able men.\nThus he is surrounded by men who suggest to him the lines of action.\nThis brings blessing, fame, and good fortune to him and all the people.",
        },
        {
          .pd ="His house is in a state of abundance.\nHe screens off his family.\nHe peers through the gate\nAnd no longer perceives anyone.\nFor three years he sees nothing.\nMisfortune.",
          .cm = "This describes a man who because of his arrogance and obstinacy attains\nthe opposite of what he strives for. He seeks abundance and splendor\nfor his dwelling. He wishes at all odds to be master in his house, which\nso alienates his family that in the end he finds himself completely isolated.",
        }
      }
  },
  {
    .number = 56,
    .sk = 0b00101100,
    .name = "The Wanderer",
    .cm = "The mountain, Kên, stands still; above it fire, Li, flames up\nand does not tarry. Therefore the two trigrams do not stay together.\nStrange lands and separation are the wanderer's lot. When a man is a\nwanderer and stranger, he should not be gruff nor overbearing. He has\nno large circle of acquaintances, therefore he should not give himself\nairs. He must be cautious and reserved; in this way he protects himself\nfrom evil. If he is obliging toward others, he wins success. A wanderer\nhas no fixed abode; his home is the road. Therefore he must take care to\nremain upright and steadfast, so that he sojourns only in the proper places,\nassociating only with good people. Then he has good fortune and can go his\nway unmolested.",
    .jd = "THE WANDERER. Success through smallness.\nPerseverance brings good fortune\nto the wanderer.",
    .j_cm = "The mountain, KĂŞn, stands still; above it fire, Li,\nflames up and does not tarry.\nTherefore the two trigrams do not stay together.\nStrange lands and separation are the wanderer's lot.\nWhen a man is a wanderer and stranger, he should not be gruff nor overbearing.\nHe has no large circle of acquaintances,\ntherefore he should not give himself airs.\nHe must be cautious and reserved; in this way he protects himself from evil.\nIf he is obliging toward others, he wins success.\nA wanderer has no fixed abode; his home is the road.\nTherefore he must take care to remain upright and steadfast,\nso that he sojourns only in the proper places,\nassociating only with good people.\nThen he has good fortune and can go his way unmolested.",
    .im =   "Fire on the mountain:\nThe image of THE WANDERER.\nThus the superior man\nIs clear-minded and cautious\nIn imposing penalties,\nAnd protracts no lawsuits.",
    .i_cm = "When grass on a mountain takes fire, there is bright light. However,\nthe fire does not linger in one place, but travels on to new fuel. It\nis a phenomenon of short duration. This is what penalties and lawsuits\nshould be like. They should be a quickly passing matter, and must not be\ndragged out indefinitely. Prisons ought to be places where people are lodged\nonly temporarily, as guests are. They must not become dwelling places.",
        .lines = {
        {
          .pd ="If the wanderer busies himself with trivial things,\nHe draws down misfortune upon himself.",
          .cm = "A wanderer should not demean himself or busy himself with inferior\nthings he meets with along the way. The humbler and more defenseless\nhis outward position, the more should he preserve his inner dignity. For\na stranger is mistaken if he hopes to find a friendly reception through\nlending himself to jokes and buffoonery. The result will be only contempt\nand insulting treatment.",
        },
        {
          .pd ="The wanderer comes to an inn.\nHe has his property with him.\nHe wins the steadfastness of a young servant.",
          .cm = "The wanderer her described is modest and reserved. He does not lose\ntouch with his inner being, hence he finds a resting place. In the outside\nworld he does not lose the liking of other people, hence all persons\nfurther him, so that he can acquire property. Moreover, he wins the allegiance\nof a faithful and trustworthy servant-a thing of inestimable value to\na wanderer.",
        },
        {
          .pd ="The wanderer's inn burns down.\nHe loses the steadfastness of his young servant.\nDanger.",
          .cm = "A truculent stranger does not know how to behave properly. He meddles\nin affairs and controversies that do not concern him; thus he loses his\nresting place. He treats his servant with aloofness and arrogance; thus\nhe loses the man's loyalty. When a stranger in a strange land has no one\nleft on whom he can rely, the situation becomes very dangerous.",
        },
        {
          .pd ="The wanderer rests in a shelter.\nHe obtains his property and an ax.\nMy heart is not glad.",
          .cm = "This describes a wanderer who knows how to limit his desires outwardly,\nthough he is inwardly strong and aspiring. Therefore he finds at least\na place of shelter in which he can stay. He also succeeds in acquiring\nproperty, but even with this he is not secure. He must be always on guard,\nready to defend himself with arms. Hence he is not at ease. He is persistently\nconscious of being a stranger in a strange land.",
        },
        {
          .pd ="He shoots a pheasant.\nIt drops with the first arrow.\nIn the end this brings both praise and office.",
          .cm = "Traveling statesman were in the habit of introducing themselves to\nlocal princes with the gift of a pheasant, killing it at the first shot.\nThus he finds friends who praise and recommend him, and in the end the\nprince accepts him and confers an office upon him. Circumstances often\ncause a man to seek a home in foreign parts. If he knows how to meet the\nsituation and how to introduce himself in the right way, he may find a circle\nof friends and a sphere of activity even in a strange country.",
        },
        {
          .pd ="The bird's nest burns up.\nThe wanderer laughs at first,\nThen must needs lament and weep.\nThrough carelessness he loses his cow.\nMisfortune.",
          .cm = "The picture of a bird whose nest burns up indicates loss of one's\nresting place. This misfortune may overtake the bird if it is heedless and\nimprudent when building its nest. It is the same with a wanderer. If he lets\nhimself go, laughing and jesting, and forgets that he is a wanderer, he will\nlater have cause to weep and lament. For if through carelessness a man loses\nhis cow-i.e., his modesty and adaptability-evil will result.",
        }
      }
  },
  {
    .number = 57,
    .sk = 0b00110110,
    .name = "The Gentle (The Penetrating, Wind)",
    .cm = "Sun is one of the eight doubled trigrams. It is the eldest daughter\nand symbolizes wind or wood; it has for its attribute gentleness, which\nnonetheless penetrates like the wind or like growing wood with its roots.\nThe dark principle, in itself rigid and immovable, is dissolved by the\npenetrating light principle, to which it subordinates itself in gentleness.\nIn nature, it is the wind that disperses the gathered clouds, leaving the\nsky clear and serene. In human life it is penetrating clarity of judgment\nthat thwarts all dark hidden motives. In the life of the community it is\nthe powerful influence of a great personality that uncovers and breaks up\nthose intrigues which shun the light of day.",
    .jd = "THE GENTLE. Success through what is small.\nIt furthers one to have somewhere to go.\nIt furthers one to see the great man.",
    .j_cm = "Penetration produces gradual and inconspicuous effects. It should\nbe effected not by an act of violation but by influence that never lapses.\nResults of this kind are less striking to the eye than those won by surprise\nattack, but they are more enduring and more complete. If one would produce\nsuch effects, one must have a clearly defined goal, for only when the penetrating\ninfluence works always in the same direction can the object be attained.\nSmall strength can achieve its purpose only by subordinating itself to an\neminent man who is capable of creating order.",
    .im =   "Winds following one upon the other:\nThe image of THE GENTLY PENETRATING.\nThus the superior man\nSpreads his commands abroad\nAnd carries out his undertakings.",
    .i_cm = "The penetrating quality of the wind depends upon its ceaselessness.\nThis is what makes it so powerful; time is its instrument. In the same\nway the ruler's thought should penetrate the soul of the people. This\ntoo requires a lasting influence brought about by enlightenment and command.\nOnly when the command has been assimilated by the people is action in\naccordance with it possible. Action without preparation of the ground\nonly frightens and repels.",
        .lines = {
        {
          .pd ="In advancing and in retreating,\nThe perseverance of a warrior furthers.",
          .cm = "In born gentleness is often carried to the point of indecisiveness.\nOne does not feel strong enough to advance resolutely. A thousand doubts\ncrop up; one is, however, not minded to withdraw but drifts indecisively\nto and fro. In such a situation, a military decisiveness is the proper\nthing, so that one resolutely does what order demands. Resolute discipline\nis far better than irresolute license.",
        },
        {
          .pd ="Penetration under the bed.\nPriests and magicians are used in great number.\nGood fortune. No blame.",
          .cm = "At times one has to deal with hidden enemies, intangible influences\nthat slink into dark corners and from this hiding affect people by suggestion.\nIn instances like this, it is necessary to trace these things back to\nthe most secret recesses, in order to determine the nature of the influences\nto be dealt with. This is the task of the priests; removing the influences\nis the task of the magicians. The very anonymity of such plotting requires\nan especially vigorous and indefatigable effort, but this is well worth\nwhile. For when such elusive influences are brought into the light and\nbranded, they lose their power over people.",
        },
        {
          .pd ="Repeated penetration. Humiliation.",
          .cm = "Penetrating reflection must not be pushed too far, lest it cripple\nthe power of decision. After a matter has been thoroughly pondered, it\nis essential to form a decision and to act. Repeated deliberation brings\nfresh doubts and scruples, and thereby humiliation, because one shows\noneself unable to act.",
        },
        {
          .pd ="Remorse vanishes.\nDuring the hunt\nThree kinds of game are caught.",
          .cm = "When a responsible position and accumulated experience lead one to\ncombine innate modesty with energetic action, great success is assured.\nThe three kinds of animals referred to served for offerings to the gods,\nfor feasting guests, and for everyday consumption. When the catch answered\nall three purposes, the hunt was considered especially successful.",
        },
        {
          .pd ="Perseverance brings good fortune.\nRemorse vanishes.\nNothing that does not further.\nNo beginning, but an end.\nBefore the change, three days.\nAfter the change, three days.\nGood fortune.",
          .cm = "In the situation described in Ku, WORK ON WHAT HAS BEEN SPOILED 18,\nan entirely new point of departure must be set\nup, whereas here it is only a question of reforms. The beginning has\nnot been good, but the moment has been reached when a new direction can\nbe taken. Change and improvement are called for. Such steps must be undertaken\nwith steadfastness, that is, with a firm and correct attitude of mind;\nthen they will succeed, and remorse will disappear. But it must be remembered\nthat such improvements require careful consideration. Before a change is\nmade, it must be pondered over again and again. After the change is made,\nit is necessary to note carefully for some time after how the improvements\nbear the test of actuality. Such careful work is accompanied by good fortune.",
        },
        {
          .pd ="Penetration under the bed.\nHe loses his property and his ax.\nPerseverance brings misfortune.",
          .cm = "A man's understanding is sufficiently penetrating. He follows up injurious\ninfluences into the most secret corners. But he no longer has the strength\nto combat them decisively. In this case any attempt to penetrate into\nthe personal domain of darkness would only bring harm.",
        }
      }
  },
  {
    .number = 58,
    .sk = 0b00011011,
    .name = "The Joyous, Lake",
    .cm = "This hexagram, like sun, is one of the eight formed by doubling of\na trigram. The trigram Tui denotes the youngest daughter; it is symbolized\nby the smiling lake, and its attribute is joyousness. Contrary to appearances,\nit is not the yielding quality of the top line that accounts for joy\nhere. The attribute of the yielding or dark principle is not joy but\nmelancholy. However, joy is indicated by the fact that there are two\nstrong lines within, expressing themselves through the medium of gentleness.\nTrue joy, therefore, rests on firmness and strength within, manifesting\nitself outwardly as yielding and gentle.",
    .jd = "THE JOYOUS. Success.\nPerseverance is favorable.",
    .j_cm = "The joyous mood is infectious and therefore brings success. But joy\nmust be based on steadfastness if it is not to degenerate into uncontrolled\nmirth. Truth and strength must dwell in the heart, while gentleness reveals\nitself in social intercourse. In this way one assumes the right attitude\ntoward God and man and achieves something.\nUnder certain conditions, intimidation without gentleness\nmay achieve something momentarily, but not for all time.\nWhen, on the other hand, the hearts of men are won by friendliness,\nthey are led to take all hardships upon themselves willingly, and if need\nbe will not shun death itself, so great is the power of joy over men.",
    .im =   "Lakes resting one on the other:\nThe image of THE JOYOUS.\nThus the superior man joins with his friends\nFor discussion and practice.",
    .i_cm = "A lake evaporates upward and thus gradually dries up; but when two\nlakes are joined they do not dry up so readily, for one replenishes the\nother. It is the same in the field of knowledge. Knowledge should be a\nrefreshing and vitalizing force. It becomes so only through stimulating\nintercourse with congenial friends with whom one holds discussion and practices\napplication of the truths of life. In this way learning becomes many-sided\nand takes on a cheerful lightness, whereas there is always something ponderous\nand one-sided about the learning of the self-taught.",
        .lines = {
        {
          .pd ="Contented joyousness. Good fortune.",
          .cm = "A quiet, wordless, self-contained joy, desiring nothing from without\nand resting content with everything, remains free of all egotistic likes\nand dislikes. In this freedom lies good fortune, because it harbors the\nquiet security of a heart fortified within itself.",
        },
        {
          .pd ="Sincere joyousness. Good fortune.\nRemorse disappears.",
          .cm = "We often find ourselves associating with inferior people in whose\ncompany we are tempted by pleasures that are inappropriate for the superior\nman. To participate in such pleasures would certainly bring remorse, for\na superior man can find no real satisfaction in low pleasures. When, recognizing\nthis, a man does not permit his will to swerve, so that he does not find\nsuch ways agreeable, not even dubious companions will venture to proffer\nany base pleasures, because he would not enjoy them. Thus every cause for\nregret is removed.",
        },
        {
          .pd ="Coming joyousness. Misfortune.",
          .cm = "True joy must spring from within. But if one is empty within and wholly\ngiven over to the world, idle pleasures come streaming in from without.\nThis is what many people welcome as diversion. Those who lack inner stability\nand therefore need amusement, will always find opportunity of indulgence.\nThey attract external pleasures by the emptiness of their natures. Thus\nthey lose themselves more and more, which of course has bad results.",
        },
        {
          .pd ="Joyousness that is weighed is not at peace.\nAfter ridding himself of mistakes a man has joy.",
          .cm = "Often a man finds himself weighing the choice between various kinds\nof pleasures, and so long as he has not decided which kind he will choose,\nthe higher or the lower, he has no inner peace. Only when he clearly\nrecognizes that passion brings suffering, can he make up his mind to\nturn away from the lower pleasures and to strive for the higher. Once\nthis decision is sealed, he finds true joy and peace, and inner conflict\nis overcome.",
        },
        {
          .pd ="Sincerity toward disintegrating influences is dangerous.",
          .cm = "Dangerous elements approach even the far best of men. If a man permits\nhimself to have anything to do with them, their disintegrating influence\nacts slowly but surely, and inevitable brings dangers in its train. But\nif he recognizes the situation and can comprehend the danger, he knows\nhow to protect himself and remains unharmed.",
        },
        {
          .pd ="Seductive joyousness.",
          .cm = "A vain nature invites diverting pleasures and must suffer accordingly\n(cf. the six in the third place). If a man is unstable within, the pleasures\nof the world that he does not shun have so powerful an influence that\nhe is swept along by them. Here it is no longer a question of danger, of\ngood fortune or misfortune. He has given up direction of his own life,\nand what becomes of him depends upon chance and external influences.",
        }
      }
  },
  {
    .number = 59,
    .sk = 0b00110010,
    .name = "Dispersion [Dissolution]",
    .cm = "Wind blowing over water disperses it, dissolving it into foam and\nmist. This suggests that when a man's vital energy is dammed up within him\n(indicated as a danger by the attribute of the lower trigram), gentleness\nserves to break up and dissolve the blockage.",
    .jd = "DISPERSION. Success.\nThe king approaches his temple.\nIt furthers one to cross the great water.\nPerseverance furthers.",
    .j_cm = "The text of this hexagram resembles that of Ts'ui, GATHERING TOGETHER\n45. In the latter, the subject is the bringing together\nof elements that have been separated, as water collects in lakes upon\nthe earth. Here the subject is the dispersing and dissolving of divisive\negotism. DISPERSION shows the way, so to speak, that leads to gathering\ntogether. This explains the similarity of the two texts. Religious forces\nare needed to overcome the egotism that divides men. The common celebration\nof the great sacrificial feasts and sacred rites, which gave expression\nsimultaneously to the interrelation and social articulation of the family\nand state, was the means of employed by the great ruler to unite men.\nThe sacred music and the splendor of the ceremonies aroused a strong tide\nof emotion that was shared by all hearts in unison, and that awakened a\nconsciousness of the common origin of all creatures. In this way disunity\nwas overcome and rigidity dissolved.\nA further means to the same end is co-operation\nin great general undertakings that set a high goal for the will of the people;\nin the common concentration on this goal, all barriers dissolve, just as,\nwhen a boat is crossing a great stream, all hands must unite in a joint\ntask. But only a man who is himself free of all selfish ulterior considerations,\nand who perseveres in justice and steadfastness, is capable of so dissolving\nthe hardness of egotism.",
    .im =   "The wind drives over the water:\nThe image of DISPERSION.\nThus the kings of old sacrificed to the Lord\nAnd built temples.",
    .i_cm = "In the autumn and winter, water begins to freeze into ice. When the\nwarm breezes of spring come, the rigidity is dissolved, and the elements\nthat have been dispersed in ice floes are reunited. It is the same with\nthe minds of the people. Through hardness and selfishness the heart grows\nrigid, and this rigidity leads to separation from all others. Egotism and\ncupidity isolate men. Therefore the hearts of men must be seized by a devout\nemotion. They must be shaken by a religious awe in face of eternity-stirred\nwith an intuition of the One Creator of all living beings, and united through\nthe strong feeling of fellowship experienced in the ritual of divine worship.",
        .lines = {
        {
          .pd ="He brings help with the strength of a horse.\nGood fortune.",
          .cm = "It is important that disunion should be overcome at the outset, before\nit has become complete-that the clouds should be dispersed before they\nhave brought storm and rain. At such times when hidden divergences in\ntemper make themselves felt and lead to mutual misunderstandings we must\ntake quick and vigorous action to dissolve the misunderstandings and mutual\ndistrust.",
        },
        {
          .pd ="At the dissolution\nHe hurries to that which supports him.\nRemorse disappears.",
          .cm = "When an individual discovers within himself the beginnings of alienation\nfrom others, of misanthropy and ill humor, he must set about dissolving\nthese obstructions. He must rouse himself inwardly, hasten to that which\nsupports him. Such support is never found in hatred, but always in a moderate\nand just judgment of men, linked with good will. If he regains this unobstructed\noutlook on humanity, while at the same time all saturnine ill humor is\ndissolved, all occasion for remorse disappears.",
        },
        {
          .pd ="He dissolves his self. No remorse.",
          .cm = "Under certain circumstances, a man's work may become so difficult\nthat he can no longer think of himself. He must set aside all personal desires\nand disperse whatever the self gathers about it to serve as a barrier\nagainst others. Only on the basis of great renunciation can he obtain\nthe strength for great achievements. By setting his goal in a great task\noutside himself, he can attain this standpoint.",
        },
        {
          .pd ="He dissolves his bond with his group.\nSupreme good fortune.\nDispersion leads in turn to accumulation.\nThis is something that ordinary men do not think of.",
          .cm = "When we are working at a task that affects the general welfare, we\nmust leave all private friendships out of account. Only by rising above\nparty interests can we achieve something decisive. He who has the courage\nthus to forego what is near wins what is afar. But in order to comprehend\nthis standpoint, one must have a wide view of the interrelationships of\nlife, such as only unusual men attain.",
        },
        {
          .pd ="His loud cries are as dissolving as sweat.\nDissolution! A king abides without blame.",
          .cm = "In times of general dispersion and separation, a great idea provides\na focal point for the organization of recovery. Just as an illness reaches\nits crisis in a dissolving sweat, so a great stimulating idea is a true\nsalvation in times of general deadlock. It gives the people a rallying\npoint-a man in a ruling position who can dispel misunderstandings.",
        },
        {
          .pd ="He dissolves his blood.\nDeparting, keeping at a distance, going out,\nIs without blame.",
          .cm = "The idea of the dissolving of a man's blood means the dispersion of\nthat which might lead to bloodshed and wounds, i.e., avoidance of danger.\nBut here the thought is not that a man avoids difficulties for himself\nalone, but rather that he rescues his kin-helps them to get away before\ndanger comes, or to keep at a distance from an existing danger, or to find\na way out of a danger that is already upon them. In this way he does what\nis right.",
        }
      }
  },
  {
    .number = 60,
    .sk = 0b00010011,
    .name = "Limitation",
    .cm = "A lake occupies a limited space. When more water comes into it, it\noverflows. Therefore limits must be set for the water. The image shows\nwater below and water above, with the firmament between them as a limit.\nThe Chinese word for limitation really denotes the joints that divide\na bamboo stalk. In relation to ordinary life it means the thrift that sets\nfixed limits upon expenditures. In relation to the moral sphere it means\nthe fixed limits that the superior man sets upon his actions-the limits\nof loyalty and disinterestedness.",
    .jd = "LIMITATION. Success.\nGalling limitation must not be persevered in.",
    .j_cm = "Limitations are troublesome, but they are effective. If we live economically\nin normal times, we are prepared for times of want. To be sparing saves\nus from humiliation. Limitations are also indispensable in the regulation\nof world conditions. In nature there are fixed limits for summer and winter,\nday and night, and these limits give the year its meaning. In the same\nway, economy, by setting fixed limits upon expenditures, acts to preserve\nproperty and prevent injury to the people. But in limitation we must\nobserve due measure. If a man should seek to impose galling limitations\nupon his own nature, it would be injurious. And if he should go too far\nin imposing limitations on others, they would rebel. Therefore it is necessary\nto set limits even upon limitation.",
    .im =   "Water over lake: \nThe image of LIMITATION.\nThus the superior man\nCreates number and measure,\nAnd examines the nature of virtue and correct conduct.",
    .i_cm = "A lake is something limited. Water is inexhaustible. A lake can contain\nonly a definite amount of the infinite quantity of water; this is its\npeculiarity. In human life too the individual achieves significance through\ndiscrimination and the setting of limits. Therefore what concerns us here\nis the problem of clearly defining these discriminations, which are, so\nto speak, the backbone of morality. Unlimited possibilities are not suited\nto man; if they existed, his life would only dissolve in the boundless.\nTo become strong, a man's life needs the limitations ordained by duty and\nvoluntarily accepted. The individual attains significance as a free spirit\nonly by surrounding himself with these limitations and by determining for\nhimself what his duty is.",
        .lines = {
        {
          .pd ="Not going out of the door and the courtyard\nIs without blame.",
          .cm = "Often a man who would like to undertake something finds himself confronted\nby insurmountable limitations. Then he must know where to stop. If he\nrightly understands this and does not go beyond the limits set for him,\nhe accumulates an energy that enables him, when the proper time comes,\nto act with great force. Discretion is of prime importance in preparing\nthe way for momentous things. Concerning this, Confucius says:\nWhere disorder develops, words are the first steps. If the prince\nis not discreet, he loses his servant. If the servant is not discreet he\nloses his life. If germinating things are not handled with discretion, the\nperfecting of them is impeded. Therefore the superior man is careful to maintain\nsilence and does not go forth.",
        },
        {
          .pd ="Not going out of the gate and the courtyard\nBrings misfortune.",
          .cm = "When the time for action has come, the moment must be quickly seized.\nJust as water first collects in a lake without flowing out, yet is certain\nto find an outlet when the lake is full, so it is in the life of man.\nIt is a good thing to hesitate so long as the time for action has not come,\nbut no longer. Once the obstacles to action have been removed, anxious\nhesitation is a mistake that is bound to bring disaster, because one\nmisses one's opportunity.",
        },
        {
          .pd ="He who knows limitation\nWill have cause to lament.\nNo blame.",
          .cm = "If an individual is bent only on pleasures and enjoyment, it is easy\nfor him to lose his sense of the limits that are necessary. If he gives\nhimself over to extravagance, he will have to suffer the consequences,\nwith accompanying regret. He must not seek to lay the blame on others.\nOnly when we realize that our mistakes are of our own making will such\ndisagreeable experiences free us of errors.",
        },
        {
          .pd ="Contented limitation. Success.",
          .cm = "Every limitation has its value, but a limitation that requires persistent\neffort entails a cost of too much energy. When, however, the limitation\nis a natural one (as for example, the limitation by which water flows\nonly downhill), it necessarily leads to success, for then it means a saving\nof energy. The energy that otherwise would be consumed in a vain struggle\nwith the object, is applied wholly to the benefit of the matter in hand,\nand success is assured.",
        },
        {
          .pd ="Sweet limitation brings good fortune.\nGoing brings esteem.",
          .cm = "The limitation must be carried out in the right way if it is to be\neffective. If we seek to impose restrictions on others only, while evading\nthem ourselves, these restrictions will always be resented and will provoke\nresistance. If, however, a man in a leading position applies the limitation\nfirst to himself, demanding little from those associated with him, and\nwith modest means manages to achieve something, good fortune is the result.\nWhere such an example occurs, it meets with emulation, so that whatever\nis undertaken must succeed.",
        },
        {
          .pd ="Galling limitation.\nPerseverance brings misfortune.\nRemorse disappears.",
          .cm = "If one is too severe in setting up restrictions, people will not endure\nthem. The more consistent such severity, the worse it is, for in the\nlong run a reaction is unavoidable. In the same way, the tormented body\nwill rebel against excessive asceticism. On the other hand, although ruthless\nseverity is not to be applied persistently and systematically, there may\nbe times when it si the only means of safeguarding against guilt and remorse.\nIn such situations ruthlessness toward oneself is the only means of saving\none's soul, which otherwise would succumb to irresolution and temptation.",
        }
      }
  },
  {
    .number = 61,
    .sk = 0b00110011,
    .name = "Inner Truth",
    .cm = "The wind blows over the lake and stirs the surface of the water. Thus\nvisible effects of the invisible manifest themselves. The hexagram consists\nof firm lines above and below, while it is open in the center. This indicates\na heart free of prejudices and therefore open to truth. On the other\nhand, each of the two trigrams has a firm line in the middle; this indicates\nthe force of inner truth in the influences they present. The attributes\nof the two trigrams are: above, gentleness, forbearance toward inferiors;\nbelow, joyousness in obeying superiors. Such conditions create the basis\nof a mutual confidence that makes achievements possible. The character\nof fu (\"truth\") is actually the picture of a bird's foot over a fledgling.\nIt suggests the idea of brooding. An egg is hollow. The light-giving power\nmust work to quicken it from outside, but there must be a germ of life\nwithin, if life is to be awakened. Far-reaching speculations can be linked\nwith these ideas.",
    .jd = "INNER TRUTH. Pigs and fishes.\nGood fortune.\nIt furthers one to cross the great water.\nPerseverance furthers.",
    .j_cm = "Pigs and fishes are the least intelligent of all animals and therefore\nthe most difficult to influence. The force of inner truth must grow great\nindeed before its influence can extend to such creatures. In dealing\nwith persons as intractable and as difficult to influence as a pig or\na fish, the whole secret of success depends on finding the right way of\napproach. One must first rid oneself of all prejudice and, so to speak,\nlet the psyche of the other person act on one without restraint. Then\none will establish contact with him, understand and gain power over him.\nWhen a door has thus been opened, the force of one's personality will\ninfluence him. If in this way one finds no obstacles insurmountable, one\ncan undertake even the most dangerous things, such as crossing the great\nwater, and succeed. But it is important to understand upon what the force\ninner truth depends. This force is not identical with simple intimacy or\na secret bond. Close ties may exist also among thieves; it is true that such\na bond acts as a force but, since it is not invincible, it does not bring\ngood fortune. All association on the basis of common interests holds only\nup to a certain point. Where the community of interest ceases, the holding\ntogether ceases also, and the closest friendship often changes into hate.\nOnly when the bond is based on what is right, on steadfastness, will it remain\nso firm that it triumphs over everything.",
    .im =   "Wind over lake: \nThe image of INNER TRUTH.\nThus the superior man discusses criminal cases\nIn order to delay executions.",
    .i_cm = "Wind stirs water by penetrating it. Thus the superior man, when obliged\nto judge the mistakes of men, tries to penetrate their minds with understanding,\nin order to gain a sympathetic appreciation of the circumstances. In\nancient China, the entire administration of justice was guided by this\nprinciple. A deep understanding that knows how to pardon was considered\nthe highest form of justice. This system was not without success, for\nits aim was to make so strong a moral impression that there was no reason\nto fear abuse of such mildness. For it sprang not from weakness but from\na superior clarity.",
        .lines = {
        {
          .pd ="Being prepared brings good fortune.\nIf there are secret designs, it is disquieting.",
          .cm = "The force of inner truth depends chiefly on inner stability and preparedness.\nFrom this state of mind springs the correct attitude toward the outer\nworld. But if a man should try to cultivate secret relationships of a\nspecial sort, it would deprive him of his inner independence. The more\nreliance he places on the support of others, the more uneasy and anxious\nhe will become as to whether these secret ties are really tenable. In this\nway inner peace and the force of inner truth are lost.",
        },
        {
          .pd ="A crane calling in the shade.\nIts young answers it.\nI have a good goblet.\nI will share it with you.",
          .cm = "This refers to the involuntary influence of a man's inner being upon\npersons of kindred spirit. The crane need not show itself on a high hill.\nIt may be quite hidden when it sounds its call; yet its young will hear\nits not, will recognize it and give answer. Where there is a joyous mood,\nthere a comrade will appear to share a glass of wine. This is the echo\nawakened in men through spiritual attraction. Whenever a feeling is voiced\nwith truth and frankness, whenever a deed is the clear expression of sentiment,\na mysterious and far-reaching influence is exerted. At first it acts on\nthose who are inwardly receptive. But the circle grows larger and larger.\nThe root of all influence lies in one's own inner being: given true and vigorous\nexpression in word and deed, its effect is great. The effect is but the reflection\nof something that emanates from one's own heart. Any deliberate intention\nof an effect would only destroy the possibility of producing it. Confucius\nsays about this line:\nThe superior man abides in his room. If his words are well spoken,\nhe meets with assent at a distance of more than a thousand miles. How\nmuch more then from near by! If the superior man abides in his room and\nhis words are not well spoken, he meets with contradiction at a distance\nof more than a thousand miles. How much more then from near by! Words go\nforth from one's own person and exert their influence on men. Deeds are\nborn close at hand and become visible far away. Words and deeds are the\nhinge and bowspring of the superior man. As hinge and bowspring move, they\nbring honor or disgrace. Through words and deeds the superior man moves\nheaven and earth . Must one not, then, be cautious?",
        },
        {
          .pd ="He finds a comrade.\nNow he beats the drum, now he stops.\nNow he sobs, now he sings.",
          .cm = "Here the source of a man's strength lies not in himself but in his\nrelation to other people. No matter how close to them he may be, if his\ncenter of gravity depends on them, he is inevitably tossed to and fro\nbetween joy and sorrow. Rejoicing to high heaven, then sad unto death-this\nis the fate of those who depend upon an inner accord with other persons\nwhom they love. Here we have only the statement of the law that this is\nso. Whether this condition is felt to be an affliction of the supreme happiness\nof love, is left to the subjective verdict of the person concerned.",
        },
        {
          .pd ="The moon nearly at the full.\nThe team horse goes astray.\nNo blame.",
          .cm = "To intensify the power of inner truth, a man must always turn to his\nsuperior, from whom he can receive enlightenment as the moon receives\nlight form the sun. However, this requires a certain humility, like that\nof the moon when it is not yet quite full. At the moment when the moon\nbecomes full and stands directly opposite the sun, it begins to wane. Just\nas on the one hand we must be humble and reverent when face to face with\nthe source of enlightenment, so likewise must we on the other renounce\nfactionalism among men. Only be pursuing one's course like a horse that\ngoes straight ahead without looking sidewise at its mate, can one retain\nthe inner freedom that helps one onward.",
        },
        {
          .pd ="He possesses truth, which links together.\nNo blame.",
          .cm = "This describes the ruler who holds all elements together by the power\nof his personality. Only when the strength of his character is so ample\nthat he can influence all who are subject to him, is he as he needs to\nbe. The power of suggestion must emanate from the ruler. It will firmly\nknit together and unite all his adherents. Without this central force,\nall external unity is only deception and breaks down at the decisive moment.",
        },
        {
          .pd ="Cockcrow penetrating to heaven.\nPerseverance brings misfortune.",
          .cm = "The cock is dependable. It crows at dawn. But it cannot itself fly\nto heaven. It just crows. A man may count on mere words to awaken faith.\nThis may succeed now and then, but if persisted in, it will have bad consequences.",
        }
      }
  },
  {
    .number = 62,
    .sk = 0b00001100,
    .name = "Preponderance of the Small",
    .cm = "While in the hexagram Ta Kuo, PREPONDERANCE OF THE GREAT 28,\nthe strong lines preponderate and are within,\ninclosed between weak lines at the top and bottom, the present hexagram\nhas weak lines preponderating, though here again they are on the outside,\nthe strong lines being within. This indeed is the basis of the exceptional\nsituation indicated by the hexagram. When strong lines are outside, we\nhave the hexagram I, PROVIDING NOURISHMENT 27, or Chung Fu, INNER TRUTH 61;\nneither represents and exceptional state. When\nstrong elements within preponderate, they necessarily enforce their will.\nThis creates struggle and exceptional conditions in general. But in the\npresent hexagram it is the weak element that perforce must mediate with\nthe outside world. If a man occupies a position of authority for which\nhe is by nature really inadequate, extraordinary prudence is necessary.",
    .jd = "PREPONDERANCE OF THE SMALL. Success.\nPerseverance furthers.\nSmall things may be done; great things should not be done.\nThe flying bird brings the message:\nIt is not well to strive upward,\nIt is well to remain below.\nGreat good fortune.",
    .j_cm = "Exceptional modesty and conscientiousness are sure to be rewarded\nwith success; however, if a man is not to throw himself away, it is important\nthat they should not become empty form and subservience but be combined always\nwith a correct dignity in personal behavior. We must understand the demands\nof the time in order to find the necessary offset for its deficiencies\nand damages. In any event we must not count on great success, since the\nrequisite strength is lacking. In this lies the importance of the message\nthat one should not strive after lofty things but hold to lowly things.\nThe structure of the hexagram gives rise to the idea that this message\nis brought by a bird. In Ta Kuo, PREPONDERANCE OF THE GREAT 28,\nthe four strong, heavy lines within, supported\nonly by two weak lines without, give the image of a sagging ridgepole.\nHere the supporting weak lines are both outside and preponderant; this\ngives the image of a soaring bird. But a bird should not try to surpass\nitself and fly into the sun; it should descend to the earth, where its nest\nis. In this way it gives the message conveyed by the hexagram.",
    .im =   "Thunder on the mountain:\nThe image of PREPONDERANCE OF THE SMALL.\nThus in his conduct the superior man gives preponderance to reverence.\nIn bereavement he gives preponderance to grief.\nIn his expenditures he gives preponderance to thrift.",
    .i_cm = "Thunder on the mountain is different from thunder on the plain. In\nthe mountains, thunder seems much nearer; outside the mountains, it is\nless audible than the thunder of an ordinary storm. Thus the superior\nman derives an imperative from this image: he must always fix his eyes\nmore closely and more directly on duty than does the ordinary man, even\nthough this might make his behavior seem petty to the outside world. He\nis exceptionally conscientious in his actions. In bereavement emotion\nmeans more to him than ceremoniousness. In all his personal expenditures\nhe is extremely simple and unpretentious. In comparison with the man of\nthe masses, all this makes him stand out as exceptional. But the essential\nsignificance of his attitude lies in the fact that in external matters he\nis on the side of the lowly.",
        .lines = {
        {
          .pd ="The bird meets with misfortune through flying.",
          .cm = "A bird ought to remain in the nest until it is fledged. If it tries\nto fly before this, it invites misfortune. Extraordinary measures should\nbe resorted to only when all else fails. At first we ought to put up with\ntraditional ways as long as possible; otherwise we exhaust ourselves and\nour energy and still achieve nothing.",
        },
        {
          .pd ="She passes by her ancestor\nAnd meets her ancestress.\nHe does not reach his prince\nAnd meets the official.\nNo blame.",
          .cm = "Two exceptional situations are instanced here. In the temple of ancestors,\nwhere alternation of generations prevails, the grandson stands on the\nsame side as the grandfather. Hence his closest relations are with the\ngrandfather. The present line designates the grandson's wife, who during\nthe sacrifice passes by the ancestor and goes toward the ancestress. This\nunusual behavior is, however, an expression of her modesty. She ventures\nrather to approach the ancestress, for she feels related to her by their\ncommon sex. Hence here deviation from the rule is not a mistake. Another\nimage is that of the official who, in compliance with regulation, first\nseeks an audience with his prince. If he is not successful in this, he\ndoes not try to force anything but goes about conscientious fulfillment\nof his duty, taking his place among the other officials. This extraordinary\nrestraint is likewise not a mistake in exceptional times. (The rule is\nthat every official should first have an audience with the prince by whom\nhe is appointed. Here the appointment is made by the minister.)",
        },
        {
          .pd ="If one is not extremely careful,\nSomebody may come up from behind and strike him.\nMisfortune.",
          .cm = "At certain times extraordinary caution is absolutely necessary. But\nit is just in such life situations that we find upright and strong personalities\nwho, conscious of being in the right, disdain to hold themselves on guard,\nbecause they consider it petty. Instead, they go their way proud and\nunconcerned. But this self-confidence deludes them. There are dangers\nlurking for which they are unprepared. Yet such danger is not unavoidable;\none can escape it if he understands that the time demands that he pay\nespecial attention to small and insignificant thing.",
        },
        {
          .pd ="No blame. \nHe meets him without passing by.\nGoing brings danger. \nOne must be on guard.\nDo not act. Be constantly persevering.",
          .cm = "Hardness of character is tempered by yielding position so that no\nmistakes are made. The situation here calls for extreme caution; one must\nmake no attempt of one's own initiative to reach the desired end. And if\none were to go on, endeavoring one must be on guard and not act but continue\ninwardly to persevere.",
        },
        {
          .pd ="Dense clouds,\nNo rain from our western territory.\nThe prince shoots and hits him who is in the cave.",
          .cm = "As a high place is pictured here, the image of a flying bird has become\nthat of flying clouds. But dense as the clouds are, they race across\nthe sky and give no rain. Similarly, in exceptional times there may be\na born ruler who is qualified to set the world in order, but who cannot\nachieve anything or confer blessing on the people because he stands alone\nand has no helpers. Is such times a man must seek out helpers with whose\naid he can carry out the task. But these helpers must be modestly sought\nout in the retirement to which they have withdrawn. It is not in the fame\nnor their great names but their genuine achievements that are important.\nThrough such modesty the right man is found, and the exceptional task is\ncarried out in spite of all difficulties.",
        },
        {
          .pd ="He passes him by, not meeting him.\nThe flying bird leaves him.\nMisfortune.\nThis means bad luck and injury.",
          .cm = "If one overshoots the goal, one cannot hit it. If a bird will not\ncome to its nest but flies higher and higher, it eventually falls into the\nhunter's net. He who in times of extraordinary salience of small things does\nnot know how to call a halt, but restlessly seeks to press on and on, draws\nupon himself misfortune at the hands of gods and men, because he deviates\nfrom the order of nature.",
        }
      }
  },
  {
    .number = 63,
    .sk = 0b00010101,
    .name = "After Completion",
    .cm = "This hexagram is the evolution of T'ai PEACE 11. The transition\nfrom confusion to order is completed,\nand everything is in its proper place even in particulars. The strong\nlines are in the strong places, the weak lines in the weak places. This\nis a very favorable outlook, yet it gives reason for thought. For it is\njust when perfect equilibrium has been reached that any movement may cause\norder to revert to disorder. The one strong line that has moved to the top,\nthus effecting complete order in details, is followed by the other lines.\nEach moving according to its nature, and thus suddenly there arises again\nthe hexagram P'i, STANDSTILL 12.\nHence the present hexagram indicates the conditions\nof a time of climax, which necessitate the utmost caution.",
    .jd = "AFTER COMPLETION. Success in small matters.\nPerseverance furthers.\nAt the beginning good fortune.\nAt the end disorder.",
    .j_cm = "The transition from the old to the new time is already accomplished.\nIn principle, everything stands systematized, and it si only in regard\nto details that success is still to be achieved. In respect to this, however,\nwe must be careful to maintain the right attitude. Everything proceeds\nas if of its own accord, and this can all too easily tempt us to relax\nand let thing take their course without troubling over details.\nSuch indifference is the root of all evil.\nSymptoms of decay are bound to be the result.\nHere we have the rule indicating the usual course of history. But this\nrule is not an inescapable law. He who understands it is in position to\navoid its effects by dint of unremitting perseverance and caution.",
    .im = "Water over fire: the image of the condition\nIn AFTER COMPLETION.\nThus the superior man\nTakes thought of misfortune\nAnd arms himself against it in advance.",
    .i_cm = "When water in a kettle hangs over fire, the two elements stand in\nrelation and thus generate energy (cf. the production of steam). But the\nresulting tension demands caution.\nIf the water boils over, the fire is extinguished\nan its energy is lost. If the heat is too great, the water evaporates\ninto the air. These elements here brought in to relation and thus generating\nenergy are by nature hostile to each other. Only the most extreme caution\ncan prevent damage. In life too there are junctures when all forces are\nin balance and work in harmony, so that everything seems to be in the best\nof order. In such times only the sage recognizes the moments that bode danger\nand knows how to banish it by means of timely precautions.",
        .lines = {
        {
          .pd ="He breaks his wheels.\nHe gets his tail in the water.\nNo blame.",
          .cm = "In times following a great transition, everything is pressing forward,\nstriving in the direction of development and progress. But this pressing\nforward at the beginning is not good; it overshoots the mark and leads\nwith certainty to loss and collapse. Therefore a man of strong character\ndoes not allow himself to be infected by the general intoxication but checks\nhis course in time. He may indeed not remain altogether untouched by the\ndisastrous consequences of the general pressure, but he is hit only from\nbehind like a fox that, having crossed the water, at the last minute gets\nits tail wet. He will not suffer any real harm, because his behavior has\nbeen correct.",
        },
        {
          .pd ="The woman loses the curtain of her carriage.\nDo not run after it;\nOn the seventh day you will get it.",
          .cm = "When a woman drove out in her carriage, she had a curtain that hid\nher from the glances of the curious. It was regarded as a breach of propriety\nto drive on if this curtain was lost. Applied to public life, this means\nthat a man who wants to achieve something is not receiving that confidence\nof the authorities which he needs, so to speak, for his personal protection.\nEspecially in times \"after completion\" it may happen that those who have\ncome to power grow arrogant and conceited and no longer trouble themselves\nabout fostering new talent. This as a rule results in office seeking.\nIf a man's superiors withhold their trust from him, he will seek ways and\nmeans of getting it and of drawing attention to himself. We are warned\nagainst such an unworthy procedure: \"Do not seek it.\" Do not throw yourself\naway on the world, but wait tranquilly and develop your personal worth by\nyour own efforts. Times change. When the six stages of the hexagram have\npassed, the new era dawns. That which is a man's own cannot be permanently\nlost. It comes to him of its own accord. He need only be able to wait.",
        },
        {
          .pd ="The Illustrious Ancestor\nDisciplines the Devil's Country.\nAfter three years he conquers it.\nInferior people must not be employed.",
          .cm = "\"Illustrious Ancestor\" is the dynastic title of the Emperor Wu Ting\nof the Yin dynasty. After putting his realm in order with a strong hand,\nhe waged long colonial wars for the subjection of the Huns who occupied\nthe northern borderland with constant threat of incursions. The situation\ndescribed is as follows. After times of completion, when a new power has\narisen and everything within the country has been set in order, a period\nof colonial expansion almost inevitably follows. Then as a rule long-drawn-out\nstruggles must be reckoned with. For this reason, a correct colonial policy\nis especially important. The territory won at such bitter cost must not\nbe regarded as an almshouse for people who in one way or another have hade\nthemselves impossible at home, but who are thought to be quite good enough\nfor the colonies. Such a policy ruins at the outset any chance of success.\nThis holds true in small as well as large matters, because it is not only\nrising states that carry on a colonial policy; the urge to expand, with\nits accompanying dangers, is part and parcel of every ambitious undertaking.",
        },
        {
          .pd ="The finest clothes turn to rags.\nBe careful all day long.",
          .cm = "In a time of flowering culture, an occasional convulsion is bound\nto occur, uncovering a hidden evil within society and at first causing a\ngreat sensation. But since the situation is favorable on the whole, such\nevils can easily be glossed over and concealed from the public. Then everything\nis forgotten and peace apparently reigns complacently once more. However,\nto the thoughtful man, such occurrences are grave omens that he does\nnot neglect. This is the only way of averting evil consequences.",
        },
        {
          .pd ="The neighbor in the east who slaughters an ox\nDoes not attain as much real happiness\nAs the neighbor in the west\nWith his small offering.",
          .cm = "Religious attitudes are likewise influenced by the spiritual atmosphere\nprevailing in times after completion. In divine worship the simple old\nforms are replaced by an ever more elaborate ritual and an ever greater\noutward display. But inner seriousness is lacking in this show of magnificence;\nhuman caprice takes the place of conscientious obedience to the divine\nwill. However, while man sees what is before his eyes, God looks into the\nheart. Therefore a simple sacrifice offered with real piety holds a greater\nblessing than an impressive service without warmth.",
        },
        {
          .pd ="He gets his head in the water. Danger.",
          .cm = "Here in conclusion another warning is added. After crossing a stream,\na man's head can get into the water only if he is so imprudent as to\nturn back. As long as he goes forward and does not look back, he escapes\nthis danger. But there is a fascination in standing still and looking\nback on a peril overcome. However, such vain self-admiration brings misfortune.\nIt leads only to danger, and unless one finally resolves to go forward without\npausing, one falls a victim to this danger.",
        }
      }
  },
  {
    .number = 64,
    .sk = 0b00101010,
    .name = "Before Completion",
    .cm = "This hexagram indicates a time when the transition from disorder to\norder is not yet completed. The change is indeed prepared for, since\nall the lines in the upper trigram are in relation to those in the lower.\nHowever, they are not yet in their places. While the preceding hexagram\noffers an analogy to autumn, which forms the transition from summer to\nwinter, this hexagram presents a parallel to spring, which leads out of\nwinter's stagnation into the fruitful time of summer. With this hopeful\noutlook the Book of Changes come to its close.",
    .jd = "BEFORE COMPLETION. Success.\nBut if the little fox, \nafter nearly completing the crossing,\nGets his tail in the water,\nThere is nothing that would further.",
    .j_cm = "The conditions are difficult. The task is great and full of responsibility.\nIt is nothing less than that of leading the world out of confusion back\nto order. But it is a task that promises success, because there is a goal\nthat can unite the forces now tending in different directions. At first,\nhowever, one must move warily, like an old fox walking over ice. The caution\nof a fox walking over ice is proverbial in China. His ears are constantly\nalert to the cracking of the ice, as he carefully and circumspectly searches\nout the safest spots. A young fox who as yet has not acquired this caution\ngoes ahead boldly, and it may happen that he falls in and gets his tail\nwet when he is almost across the water. Then of course his effort has been\nall in vain. Accordingly, in times \"before completion,\" deliberation and\ncaution are the prerequisites of success.",
    .im = "Fire over water:\nThe image of the condition before transition.\nThus the superior man is careful\nIn the differentiation of things,\nSo that each finds its place.",
    .i_cm = "When fire, which by nature flames upward, is above, and water, which\nflows downward, is below, their effects take opposite directions and\nremain unrelated. If we wish to achieve an effect, we must first investigate\nthe nature of the forces in question and ascertain their proper place.\nIf we can bring these forces to bear in the right place, they will have\nthe desired effect and completion will be achieved. But in order to handle\nexternal forces properly, we must above all arrive at the correct standpoint\nourselves, for only from this vantage can we work correctly.",
        .lines = {
        {
          .pd ="He gets his tail in the water.\nHumiliating.",
          .cm = "In times of disorder there is a temptation to advance oneself as rapidly\nas possible in order to accomplish something tangible. But this enthusiasm\nleads only to failure and humiliation if the time for achievement has\nnot yet arrived. In such time it is wise to spare ourselves the opprobrium\nof failure by holding back.",
        },
        {
          .pd ="He brakes his wheels.\nPerseverance brings good fortune.",
          .cm = "Here again the time to act has not yet come. But the patience needed\nis not that of idle waiting without thought of the morrow. Kept up indefinitely,\nthis would not lead to any success. Instead, an individual must develop\nin himself the strength that will enable him to go forward. He must have\na vehicle, as it were, to effect the crossing. But he must for the time\nbeing use the brakes. Patience in the highest sense means putting brakes\non strength. Therefore he must not fall asleep and lose sight of the goal.\nIf he remains strong and steadfast in his resolve, all goes well in the\nend.",
        },
        {
          .pd ="Before completion, attack brings misfortune.\nIt furthers one to cross the great water.",
          .cm = "The time of transition has arrived, but one lacks the strength to\ncomplete the transition. If one should attempt to force it, disaster would\nresult, because collapse would then be unavoidable. What is to be done? A\nnew situation must be created; one must engage the energies of able helpers\nand in this fellowship take the decisive step-cross the great water. Then\ncompletion will become possible.",
        },
        {
          .pd ="Perseverance brings good fortune.\nRemorse disappears.\nShock, thus to discipline the Devil's Country.\nFor three years, great realms are rewarded.",
          .cm = "Now it is the time of struggle. The transition must be completed.\nWe must make ourselves strong in resolution; this brings good fortune. All\nmisgivings that might arise in such grave times of struggle must be silenced.\nIt is a question of a fierce battle to break and to discipline the Devil's\nCountry, the forces of decadence. But the struggle also has its reward. Now\nis the time to lay the foundations of power and mastery for the future.",
        },
        {
          .pd ="Perseverance brings good fortune.\nNo remorse.\nThe light of the superior man is true.\nGood fortune.",
          .cm = "The victory has been won. The power of steadfastness has not been\nrouted. Everything has gone well. All misgivings have been overcome. Success\nhas justified the deed. The light of a superior personality shines forth\nanew and makes its influence felt among men who have faith in it and rally\naround it. The new time has arrived, and with it good fortune. And just as\nthe sun shines forth in redoubled beauty after rain, or as a forest grows\nmore freshly green from charred ruins after a fire, so the new era appears\nall the more glorious by contrast with the misery of the old.",
        },
        {
          .pd ="There is drinking of wine\nIn genuine confidence. No blame.\nBut if one wets his head,\nHe loses it, in truth.",
          .cm = "Before completion, at the dawning of the new time, friends foregather\nin an atmosphere of mutual trust, and the time of waiting is passed in\nconviviality. Since the new era is hard on the threshold, there is no\nblame in this. But one must be careful in all this to keep within proper\nbounds. If in his exuberance a man gets drunk, he forfeits the favorableness\nof the situation through his intemperance.",
        }
      }
    },
  };


// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

int hexagram_number_from_user_lines(char *user_lines) {
  // 6 = old yin = -+-     sk 0   + means -- is appearing
  // 7 = young yang = ---  sk 1
  // 8 = young yin = - -   sk 0
  // 9 = old yang = -o-    sk 1   o means -- is disappearing
  // .sk = 0b00111111, // rightmost bit is the foundation (bottom line in the hexagram)
 //  note that user_lines stores the foundation in the rightmost character to align with our sk notation
  uint8_t sk = 0;
  for(int i = 0; i < 6; i++) {
    uint8_t ci = (user_lines[i] - 54) % 4; // 54 is ascii for '6', so this turns "6789" into 0 1 2 3
   // printf("ci %d ",ci);
    sk += (ci % 2) << (5 - i);
  }
  //printf("\nsk from %s is %d\n",user_lines, sk);
  for(int i=0; i < 64; i++) {
    const i_ching_hexagram *hex = &hexagrams[i];
    if(hex->sk == sk) {
      return hex->number;
    }
  }
  printf("hexagram_number_from_user_lines FAIL");
  return 1;
}

char *change_user_lines(char *user_lines) {
  for(int i = 0; i < 6; i++) {
    uint8_t c = user_lines[i];
    if(c == '6') changed_user_lines[i] = '7'; // old yin becomes young yang
    else if(c == '9') changed_user_lines[i] = '8'; // old yang becomes young yin
    else changed_user_lines[i] = c;
  }
  return changed_user_lines;
}

#define HEX_LINE_WIDTH 18

void draw_multiple_user_lines(char *user_lines[], int how_many, int width, int height) { // maybe also pass in colors?
  int i = 0;
  MU_FLOAT height_fac = (MU_FLOAT) 6 / height; // need to clamp this
  MU_FLOAT y = 0;
  MU_FLOAT width_fac = (MU_FLOAT) 12 / (MU_FLOAT)width; // need to clamp this
  int space_x = 1;

  while(i < 6) { // rightmost bit is the foundation, so we print it last
    for(int n = 0; n < how_many; n++) {
      uint8_t ci = (user_lines[n][i] - 54) % 4; // 54 is ascii for '6', so this turns "6789" into 0 1 2 3
      MU_FLOAT x = 0;
      printf("  "); // leading space
      mu_ansi_term_set_colors(MU_ANSI_TERM_BLACK, MU_ANSI_TERM_YELLOW);

      while(x < HEX_LINE_WIDTH) {
        int xi = (int)(x + 0.02);
        // make the broken lines the default color in their middle section
        if((width_fac <= 1) && xi == 6 && ((ci == 0) || (ci == 2)))
          mu_ansi_term_set_colors(MU_ANSI_TERM_YELLOW, MU_ANSI_TERM_DEFAULT_COLOR);
        if(xi == 11)
          mu_ansi_term_set_colors(MU_ANSI_TERM_BLACK, MU_ANSI_TERM_YELLOW);
        putchar(_line_strings[ci][xi]);
        x += width_fac;
      }
      mu_ansi_term_set_colors(MU_ANSI_TERM_DEFAULT_COLOR, MU_ANSI_TERM_DEFAULT_COLOR);
      // space between hexagrams
      int s = space_x;
      while(s--)
        putchar(' ');
    }
    y += (height_fac);
    printf("\n");
    if((int)(y  + 0.02)  > i) {
      i = (int)(y  + 0.02); 
      if(height_fac < 1)
        printf("\n");
    }
  }
}


void print_analaysis_of_changing_lines(char *user_lines) {
  const i_ching_hexagram *hex = &hexagrams[hexagram_number_from_user_lines(user_lines) - 1]; // this is 0-indexed
  uint8_t change_count = 0;
  for(int i = 5; i >= 0; i--) {
    uint8_t c = user_lines[i];
    if(c == '6' || c == '9') {
      printf("Line %d becoming %s:\n\n",6 - i,(c == '9' ? "broken" : "whole"));
      printf("%s\n\n",hex->lines[i].pd);
      printf("%s\n\n",hex->lines[i].cm);
      change_count++;
    }
  }
  if(change_count == 6 && (hex->lines[6].pd != NULL)) { // there is not always text for this case -- our use of designated initializers makes this safe
    printf("All lines changing means:\n\n");
    printf("%s\n\n",hex->lines[6].pd);
    printf("%s\n\n",hex->lines[6].cm);
  }
}

const i_ching_hexagram *get_hexagram(int number) {
  return &hexagrams[number - 1];
}

// =============================================================================
// Private code


