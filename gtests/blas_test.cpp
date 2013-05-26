/*!
 * \file
 * \brief BLAS aided routines test program
 * \author Adam Piatyszek
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 1995-2012  (see AUTHORS file for a list of contributors)
 *
 * This file is part of IT++ - a C++ library of mathematical, signal
 * processing, speech processing, and communications classes and functions.
 *
 * IT++ is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * IT++ is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with IT++.  If not, see <http://www.gnu.org/licenses/>.
 
 * -------------------------------------------------------------------------
 */

#include <itpp/itbase.h>
#include <iomanip>
#include "gtest/gtest.h"

using namespace itpp;

TEST(BLAS, All)
{
  int i,j;

  const double eps = 4*std::numeric_limits<double>::epsilon();

  // dot() tests
  {
    vec a = "-0.53867916598526822 -1.2749137275918296 1.0282326809755016 -0.12769391261256544 -0.36545017988288314 "
    "-0.87843445720393964 1.290111844695397 -0.67538252183750369 -0.70127933871039161 0.81090935639334638";
    vec b = "-0.53196841563668684 -3.7326992066642575 -0.1127551468065689 -0.94014676063834202 0.24924688123208288 "
    "-1.2762774231089256 0.3873527275026688 -0.38752244901288929 0.014601505447491007 0.87579113195022229";
    ASSERT_NEAR(7.5409827210127167, dot(a, b), eps);

    cvec c = "0.27048736281775243-0.43161417341950425i 0.24733404881732354+0.91695158262351717i "
    "-0.13822544249254176-0.32994867677758688i 0.45312345826746936+1.0235294160253086i 0.41375330826825513+0.17986200239570888i "
    "0.48013423494175655+0.041530819390519759i";
    cvec d = "0.3881730386544478+0.90716001255184286i -1.0369420852527764-0.10653084374544097i "
    "-0.45368954946988371+0.63018137700832588i -0.36010935124332805-1.2392806118375086i -1.0154675556525383-0.10484826487656193i "
    "0.11897334710181541+1.5931245965356391i";
    std::complex<double> actual = dot(c, d);
    ASSERT_NEAR(1.3033212438765029, actual.real(), eps);
    ASSERT_NEAR(-1.2230531450688598, actual.imag(), eps);
  }

  // outer_product() tests
  {
    vec a = "1.144350801849226 0.4030712283482284 1.288501360291362 1.97906140519714";
    vec b = "1.144800467126237 -0.8485065927711128 -1.229839122249251 1.070565791232947 1.030604680915006 "
    "-0.1875783677316915 0.4566649855427434";
    mat outer_prod = outer_product(a, b);
    const mat outer_prod_expect = "1.310053332513278 -0.970989199811978 -1.407367385691479 1.2251028216297744 1.1793732929946534 "
    "-0.2146554555233302 0.5225849423823038; 0.461436130498198 -0.3420085946098225 -0.4957127656757127 0.431514268499857 "
    "0.4154070946778456 -0.07560744309316859 0.1840685166663396; 1.475076959154343 -1.093301899001768 -1.584649381957695 "
    "1.3794254782850506 1.327935533281631 -0.2416949819835179 0.5884134550692601; 2.265630421141193 -1.679246649808636 "
    "-2.4339271414450203 2.118715439153464 2.0396299480144027 -0.3712291080276673 0.9037680479925535";
    for (i = 0; i < outer_prod.rows(); ++i) {
      for (j = 0; j < outer_prod.cols(); ++j) {
        ASSERT_NEAR(outer_prod_expect(i,j), outer_prod(i,j), eps) << i << "x" << j;
      }
    }

    cvec c = "0.648281980978702+0.5393189597808304i -1.698775822486131+0.8257068048973116i "
    "-0.3091289715393962-0.6238632012662513i 0.04914321647156545-1.10390652648413i";
    cvec d = "-0.9635435410370933+1.580133781005509i 0.4497496320949741-1.340916266239779i "
    "0.1034133351433764+0.3939126587361242i 0.6689829815540316+0.03429401822619i -0.5558754225740515-0.2154901642550677i "
    "-0.7622797901336434+0.4460947554352002i -0.7594798687045069+0.04215845908992501i";
    cmat c_outer_prod = outer_product(c, d);
    cmat c_outer_prod_expect = "-1.476844022629202+0.5047149575059551i 1.014746148300605-0.6267333496612161i -0.1454035635877053+0.3111392510750342i 0.4151941982864324+0.3830273997941929i -0.2441460888946358-0.4394925452350682i -0.7347596118728638-0.1219167517190851i -0.5150939700995433-0.3822713233903702i; 0.3321172558021201-3.479897522114934i 0.3431798846567065+2.6492774647873953i -0.5009324363178334-0.5837802062871459i -1.164768918935312+0.4941259512111063i 1.122239423196909-0.09292063807448886i 0.9265990022569888-1.387234595020226i 1.2553755120656216-0.6987254667870634i; 1.283646542921267+0.1126542274539301i -0.9755789557060787+0.1339338210560608i 0.213779554362088-0.1862855893876137i -0.1854072450702826-0.4279551390491551i 0.03740081397078696+0.4134044734853026i 0.5139448697431694+0.337657497176539i 0.2610783419657928+0.4607791410871117i; 1.696968364753062+1.1413148599564635i -1.4581440742427734-0.5623784924955431i 0.4399248187585209-0.0948005205311046i 0.0707333660175599-0.7368093610829385i -0.265198504937086+0.6030446271021392i 0.4549860312168223+0.8634081664686641i 0.009215714542340728+0.8404665860773792i";
    for (i = 0; i < c_outer_prod.rows(); ++i) {
      for (j = 0; j < c_outer_prod.cols(); ++j) {
        ASSERT_NEAR(c_outer_prod_expect(i,j).real(), c_outer_prod(i,j).real(), eps) << i << "x" << j;
        ASSERT_NEAR(c_outer_prod_expect(i,j).imag(), c_outer_prod(i,j).imag(), eps) << i << "x" << j;
      }
    }
    c_outer_prod = outer_product(c, d, true);
    c_outer_prod_expect = "0.2275481915436817-1.544029558017281i -0.4316169834226605+1.1118503571477614i 0.2794855671204302-0.1995937064010499i 0.4521850267593397+0.3385630116514046i -0.4765819513527226-0.1600957641056658i -0.2535848929428715-0.7003071352345676i -0.4696202574948612-0.4369324621380225i; 2.9415716870508546+1.888688604816574i -1.8712274870066554-1.90655480134563i 0.1495802893893871+0.7545583953771703i -1.108135310502037+0.6106416492482069i 0.7663760331692112-0.8250596001146373i 1.6632859526406825+0.1283953751221377i 1.3249965651746936-0.5554899247565929i; -0.6879280951728179+1.089584488687711i 0.6975176732665963-0.6950983115502394i -0.2777156702347496+0.05725404075528072i -0.2281967970600307-0.4067525898807242i 0.3062735813979007+0.2801759677792534i -0.04265933464460947+0.6134591230901429i 0.208476119469207+0.4868439432874201i; -1.7916716223869915+0.9860091470484897i 1.502348361298874-0.4305846158114135i -0.4297606909285259-0.1335167906494231i -0.0049814150609538-0.7401799978056773i 0.2105634924915269+0.6242243866810993i -0.5299077926936966+0.8195631042022999i -0.0838622817294241+0.8363229815150517i";
    for (i = 0; i < c_outer_prod.rows(); ++i) {
      for (j = 0; j < c_outer_prod.cols(); ++j) {
        ASSERT_NEAR(c_outer_prod_expect(i,j).real(), c_outer_prod(i,j).real(), eps) << i << "x" << j;
        ASSERT_NEAR(c_outer_prod_expect(i,j).imag(), c_outer_prod(i,j).imag(), eps) << i << "x" << j;
      }
    }
  }

  // Mat *= Mat operator test
  {
    mat M1 = "0.6416619010964826 0.235659520657739 1.3983669192959736 -1.916076396202661 -0.2313932978834664; "
    "-0.1723662919540427 0.5667202738083824 0.2357501419330437 1.2157243960564346 -0.8334515155604767; "
    "-0.7916949400225816 0.1902016039548307 -1.8515590721454276 -1.48074205072801 0.2367650475159956";
    mat N1 = "-0.8649851360899323 0.5426015425358679; "
    "-0.05968253198862269 0.5717199326645428; -0.3861898484143324 -0.6654857247496347; "
    "-1.3647102306443 1.6300227968022034; 0.4595579721985606 0.6434323438908552";
    M1 *= N1;
    const mat M1_expect = "1.399422553523895 -3.7198293783926872; -2.0179041413026333 1.518979524845883; "
    "3.5180970502245303 -1.349947725820544";
    for (i = 0; i < M1.rows(); ++i) {
      for (j = 0; j  < M1.cols(); ++j) {
        ASSERT_NEAR(M1_expect(i,j), M1(i,j), eps) << i << "x" << j;
      }
    }

    cmat M2 = "0.5313310759460741-0.5618900653783278i -0.6095848356402745+1.446658858924682i "
    "0.7174248000225271+0.7459764842740102i 0.2803552569457621+1.302851286217992i; "
    "0.3555319955217059-0.9194703925032131i -0.633288104876484+0.1126850099324503i -0.1945412419171371+0.07740707824065497i "
    "-1.099197860484227+0.04822719629670968i; 0.8372873214572285-0.1680261588241537i 0.8240186444425496+1.043483298307191i "
    "0.1780410762780012+0.2349504126253822i 0.0690521880409925+0.2825038112687566i; 0.2094812424070254-1.8657283920272836i "
    "-1.745273495226459+0.6962265981502513i 0.7192704358053181+1.426973623391087i 0.5700889022085341-0.350927410090021i";
    cmat N2 = "-1.443024172028881-0.1633982415054971i -0.5954993250735038-0.5638800857829505i; "
    "0.3088018358967147+0.1094460610547769i 1.0275952539185527-0.1750885564095062i; 0.2482232949362309-1.1372797157525494i "
    "-0.02110191719219557-0.3653734659787203i; 0.7222711837987867-0.1535072377450153i -0.6873480514598044-0.08690531283448709i";
    M2 *= N2;
    const cmat M2_expect = "0.2238476324597395+1.371248083267034i -0.8284149542175873+0.4305630743709985i; "
    "-1.617946452997547+1.678241138114074i -0.5691137095915012+0.705566702106269i; "
    "-0.6907886228392469+0.5673516983633202i 0.4952848265775473+0.2857432320130563i; "
    "0.9370105702181578+1.8772639329343965i -2.7644731136780063+1.91268556781121i";
    for (i = 0; i < M2.rows(); ++i) {
      for (j = 0; j < M2.cols(); ++j) {
        ASSERT_NEAR(M2_expect(i,j).real(), M2(i,j).real(), eps) << i << "x" << j;
        ASSERT_NEAR(M2_expect(i,j).imag(), M2(i,j).imag(), eps) << i << "x" << j;
      }
    }
  }

  // Vec = Mat * Vec operator test
  {
    mat M1 = "-1.156207177555769 1.937595489846581 -0.7600784760582153 1.133825535154124; "
    "-1.567451802499219 -0.3135104798924478 -0.5122172858230097 -0.9159646322420039; "
    "-1.3301111770828475 -0.8226722974807049 -0.4063568488084563 -1.296727513207429";
    vec v1 = "-0.31313016493873 -1.2622055080049717 -0.4708215799031978 0.9473031308640839";
    M1 *= v1;
    const mat M1_expect = "-0.6516625271052732; 0.2599978838689173; 0.4178069778515376";
    for (i = 0; i < M1.rows(); ++i) {
      for (j = 0; j < M1.cols(); ++j) {
        ASSERT_NEAR(M1_expect(i,j), M1(i,j), eps) << i << "x" << j;
      }
    }

    cmat M2 = "0.4598662936935902+0.2647372102970682i 1.2194828317757294+1.395466657255362i; "
    "-0.3375121080067005-0.4760783255515197i 0.09884444743078168-0.3339675294900224i; "
    "-0.3314179731182946+0.133072817036334i 0.5527689805911649+0.0704665547758981i";
    cvec v2 = "0.1158868961132298+1.145603110960835i -0.9576278296044424+0.3083997905942557i";
    M2 *= v2;
    const cmat M2_expect = "-1.8481636166122595-0.4027456260514852i; 0.5146229027137302-0.09152595306142891i; "
    "-0.7419344632949262-0.2612589613576985i";
    for (i = 0; i < M2.rows(); ++i) {
      for (j = 0; j < M2.cols(); ++j) {
        ASSERT_NEAR(M2_expect(i,j).real(), M2(i,j).real(), eps) << i << "x" << j;
        ASSERT_NEAR(M2_expect(i,j).imag(), M2(i,j).imag(), eps) << i << "x" << j;
      }
    }
  }
}
