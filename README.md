# Portable dosimeter ポータブル線量計
## Radiation counter using PIN photodiode 放射線測定器PINフォトダイオード使用
Originally it was created at the time of the Great East Japan Earthquake in 2011. The point of this repo is that it is calibrated with radioactive checking sources, so dose estimation is possible. <BR>
もともとは2011年の東日本大震災の時に作成したものです。このリポジトリのポイントは、線量が標準線源で校正されていて、その見積もりか可能なことです。<BR><BR>
<IMG align="left" alt=Overall src="img/overall.png">
Overall view. It uses PIN photodiodes S6775 as detector. The upper left box contains the PIN photodiodes and electrically shields them. Most gamma rays can be detected throught the shielding. If you would like to detect X-rays below 50 keV or beta rays, please drill a hole at the front of the detectors and cover it with black paper or aluminum foil. To reduce noise, a 9 V battery in the upper right corner is used as power source in this dosimeter. Counting is done by a PIC microcontroller in the lower half. The display contents are those of a prototype and so different from that of the uploaded firmware. The overall size is approximately 115 x 80 x 25 mm.<BR>
全体像です。検出はPINフォトダイオードS6775で行います。左上の黒いボックスがPINフォトダイオードが入っている部分です。電気的なシールドのためにアルミケースにその部分をおさめています。ガンマ線が測定対象で、ケースに窓はあけていません。ケースに穴を開けて黒い紙やアルミホイルを貼れば）50 keV以下のX線やベータ線も検出できます。ノイズを減らすには、電源を電池にするのが簡単かつ有効で、右上の9 V電池を使っています。計数や換算は下半分のPICマイコンで行います。液晶表示は試作時のもので、線量計とは違っています。全体の大きさはおよそ縦115 mm、横80 mm、厚25 mmで、秋月のプラケース112-TSに入れています。<br clear="left"/><BR>
<IMG align="left" alt=detector src="img/detector.png">
The detector case. Three PIN photodiodes S6775 are lined up on the left. They are covered with heat-shrinkable tubing for shading, but black vinyl tape or similar will also work. Since this dosimeter deals with very small signals, the detector part must be electrically shielded. We used an aluminum case with its sheet thickness of 1 mm. Gamma rays to be measured is from 0.5 MeV to 1.5 MeV. So gamma rays of 662 keV for cesium 137 and 605 keV and 796 keV for cesium 134 can be measured. In this photo taken during production, only five cables are shown, but then another zero-potential cable is also added as shown in the schematic below. The board is screwed with 5 mm spacers and grounded to the aluminum chassis.<BR>
検出器を開けたところです。左に３つ並んでいるのがPINフォトダイオードのS6775です。遮光のために熱収縮チューブをかぶせていますが、黒ビニールテープなどでもいいと思います。ごく小さい信号を扱いますので、検出器部分は電気的にシールドすることが必要になります。用いたのはYM-65（タカチ電機工業）で、アルミ板厚が1 mmです。測定対象は0.5 MeV～1.5 MeVのガンマ線です。セシウム137のガンマ線662 keV、セシウム134の605 keVや796 keVが測れます。製作中の写真で、ケーブルは赤・橙・黄・白・黒の５本しか出ていませんが、その後、ゼロ電位の緑も加えて計６本を出しています。基板は5 mmのスペーサを入れてネジ止めし、シャーシにアースをとっています。<br clear="left"/><BR>

## Schematic 回路図
<IMG alt=circuit src="img/circuit.png"><BR>
検出器部分は浜松ホトニクスさんの応用回路例を参考にしたものです。PINフォトダイオードは検出面積をかせぐため、３つ並列にしています。初段のU1により放射線が電圧パルスに変換され、U2の反転増幅で50倍されます。周波数特性は、上はオペアンプの特性で決まり（LMC662のデータシートを参照）、下はC3で20 kHz程度から下がります。出力は、数十～百 mV超で20～50 μ秒のパルスが出ているようです。それがVR1で設定するしきい値（50～60 mV）を超えた時にPICに割り込みがかかり、AD変換して電圧を記録します。左上の点線部分をアルミケースにおさめます。VR1をケース内にしてしまうと調整が面倒なのですが、ノイズ対策です。PICマイコン周辺や液晶への接続、USBコネクタの接続はごく標準的です。下のMOS FET周辺は、USBバスパワーが供給されたときに電池を節約するための部分です。必要なければ、U4のVoutをVccにつなぎ、U5, D7, D8, CN2, 10 μF, 47 kΩ x2, 10 kΩ x2を省略して構いません。RC0～RC2の回路も好みで省略できます。<BR>
The detector part is based on Hamamatsu Photonics' application circuit example. Three PIN photodiodes are used in parallel to increase the detection area. The radiation is converted into a voltage pulse by U1 and multiplied by 50 times by inverting amplifier U2. The frequency response depends on the characteristics of the op amps (see LMC662 datasheet) and also on C3 (> 20 kHz). The output seems to be a pulse of 20 to 50 μsec at tens to over a hundred mV. When it exceeds the threshold (50-60 mV) set by VR1, the PIC is interrupted to convert the pulse with A/D and recoed its voltage. The dotted line part in the upper left corner is placed in the aluminum case to minimize noise, though VR1 to adjust the threshold becomes inside the case. Circuits of the PIC microcontroller, LCD, and USB connector are very standard. The area around the MOS FET below is for saving battery power when USB bus power is supplied. If not necessary, connect Vout of U4 to Vcc and omit U5, D7, D8, CN2, 10 μF, 47 kΩ x2, and 10 kΩ x2. You can also omit circuits of RC0-RC2.<BR>

## BOM 部品表
<table>
<tr><th>Type</th><th>Part</th><th>Price(JPY)</th><th>Availability</th></tr>
<tr><td rowspan=8>IC etc</td><td>LMC662CN</td><td>289 JPY</td><td>Marutsu</td></tr>
<tr><td>PIC18F2550-I/SP</td><td>880 JPY</td><td>Akidzuki</td></tr>
<tr><td>S-812C50AY</td><td>100 JPY / 8 pcs</td><td>Akidzuki, any 78L05 compatible is ok</td></tr>
<tr><td>2SJ680</td><td>50 JPY</td><td>Akidzuki, any enhanced Pch MOS FET will work</td></tr>
<tr><td>S6775 x 3 pcs</td><td>@200 JPY x 3</td><td>Akidzuki, 1 piece will work</td></tr>
<tr><td>SD103A x 2 pcs</td><td>200 JPY / 20 pcs</td><td>Akidzuki, any Schottky Barrier will work</td></tr>
<tr><td>1N4148</td><td>100 JPY / 50 pcs</td><td>Akidzuki</td></tr>
<tr><td>3 mm LED red/green, 1 each</td><td>cheap</td><td></td></tr>
<tr><td>LCD</td><td>SD1602HULB</td><td>900 JPY</td><td>Akidzuki</td></tr>
<tr><td>Pin header</td><td>18 pins total</td><td>cheap</td><td>Akidzuki</td></tr>
<tr><td>Xtal</td><td>20 MHz</td><td>200 JPY / 5 pcs</td><td>Akidzuki P-01757</td></tr>
<tr><td>Piezo speaker</td><td>PKM13EPYH4000-A0</td><td>30 JPY</td><td>Akidzuki</td></tr>
<tr><td rowspan=6>Capacitors</td><td>Ceramic 2 pF x 1</td><td>cheap</td><td></td></tr>
<tr><td>Ceramic 15 pF x 2</td><td>cheap</td><td></td></tr>
<tr><td>Film 470 pF x 1</td><td>10</td><td>Akidzuki P-15040</td></tr>
<tr><td>Ceramic 0.1 uF x 4</td><td>cheap</td><td></td></tr>
<tr><td>Film 0.1 uF x 2</td><td>cheap</td><td>C4, C5</td></tr>
<tr><td>Electrolytic 10 uF x 2</td><td>cheap</td><td></td></tr>
<tr><td>1/4W metal glaze resistor</td><td>10 MΩ x 1</td><td>55 JPY</td><td>Marutsu</td></tr>
<tr><td rowspan=8>1/4W carbon resistors</td><td>1 MΩ x 1</td><td>cheap</td><td></td></tr>
<tr><td>68 kΩ x 1</td><td>cheap</td><td>33 kΩ x 2 will work</td></tr>
<tr><td>47 kΩ x 2</td><td>cheap</td><td>33 kΩ is also ok</td></tr>
<tr><td>33 kΩ x 1</td><td>cheap</td><td></td></tr>
<tr><td>20 kΩ x 1</td><td>cheap</td><td>10 kΩ x2 will work</td></tr>
<tr><td>10 kΩ x 4</td><td>cheap</td><td></td></tr>
<tr><td>1 kΩ x 3</td><td>cheap</td><td></td></tr>
<tr><td>560 Ω x 3</td><td>cheap</td><td></td></tr>
<tr><td>VR</td><td>5 kΩ x 1</td><td>50 JPY</td><td>Akidzuki P-14904</td></tr>
<tr><td rowspan=2>Switches</td><td>Tactile</td><td>cheap</td><td>Akidzuki</td></tr>
<tr><td>Toggle Switch, 6P or 3P</td><td>80</td><td>Akidzuki</td></tr>
<tr><td>Pin socket</td><td>16 pin</td><td>80</td><td>Akidzuki C-05779</td></tr>
<tr><td>IC socket</td><td>28 pin 300 mil</td><td>70</td><td>Akidzuki</td></tr>
<tr><td>USB connector</td><td>USB-B</td><td>50</td><td>Akidzuki, if needed</td></tr>
<tr><td>Universal PCB</td><td>glass epoxy 72 x 48 mm x 2</td><td>@60</td><td>Akidzuki</td></tr>
<tr><td>Battery snap</td><td>006P (PP3)</td><td>cheap</td><td></td></tr>
<tr><td>Baterry</td><td>9V 006P</td><td>100</td><td></td></tr>
<tr><td>Aluminum case</td><td>Takachi YM-65</td><td>540</td><td>Marutsu</td></tr>
<tr><td>Plastic case</td><td>ABS case 112-TS</td><td>120</td><td>Akidzuki</td></tr>
<tr><td>Bolts</td><td>M3x10 mm x 6</td><td>cheap</td><td></td></tr>
<tr><td>Nuts</td><td>M3 x 10</td><td>cheap</td><td></td></tr>
<tr><td>Spacer</td><td>Duracon spacer M3 5 mm x 2</td><td>@19</td><td>Marutsu</td></tr>
<tr><td>Cables</td><td></td><td></td><td></td></tr>
</table>
You may also need: tools such as screwdriver, nippers, pliers, soldering iron + solder, drill, tester, PIC write (PICkit etc.) + PC + software (download).<BR>

## PCB 基板
Detector<BR>
<IMG alt=board1 src="img/pcb_detector2.png"> <IMG alt=board1 src="img/pcb_detector.png"><BR><BR>
Controller<BR>
<IMG alt=board2 src="img/pcb_pic2.png"> <IMG alt=board2 src="img/pcb_pic.png"><BR>

## Dose rate calculation 線量率計算
Detection efficiency was examined by using checking sources in the table below by placing them at 10 mm from the PIN photodiodes. The value is calculated from actual count / number of incident photons presumed from the decay of each radionuclide. The obtained efficiency is pretty low, but if we do not use fluorescent screen or scintillator, this is about as good as it gets. The absorption coefficient of 662 keV gamma rays in 0.5 mm thick Si is calculated to be 0.0034 from the US NIST absorption coefficient (NIST Standard Reference Database 126), which is compatible with the value in the table.<BR>
以下の表の密封線源（チェッキングソース）をPINフォトダイオードから10 mmの位置において、検出効率を検討しました。効率の値は、各放射性核種の壊変の様式も含めて求めた効率（＝実際の計数値／入射しているはずのガンマ線光子数）です。低い検出効率ですが、蛍光板もシンチレータも使わなければ、こんなものかと思います。米国NISTの吸収係数(NIST Standard Reference Database 126)などを参考にして、662 keVのガンマ線が厚さ0.5 mmのSiに吸収される割合を計算すると0.0034で、表の値とまあ同じぐらいです。<BR>
<table>
<tr><th>Radionuclide</th><th>Half life (year)</th><th>Radioactivity (kBq)</th><th>Energy (keV)</th>
 <th>Dose rate (μSv/h at 10 mm)</th><th>Count (cpm)</th><th>Efficiency</th></tr>
<tr><td><sup>60</sup>Co</td><td>5.2714</td><td>9.038</td><td>1333, 1173</td>
 <td>27.6</td><td>187.8</td><td>0.00454</td></tr>
<tr><td><sup>22</sup>Na</td><td>2.6019</td><td>2.588</td><td>1275, 511</td>
 <td>7.38</td><td>44.4</td><td>0.0029</td></tr>
<tr><td><sup>137</sup>Cs</td><td>30.07</td><td>33.84</td><td>662</td>
 <td>26.4</td><td>148.2</td><td>0.00225</td></tr>
<tr><td><sup>133</sup>Ba</td><td>10.51</td><td>22.38</td><td>356, 303, etc</td>
 <td>14.2</td><td>28.8</td><td>0.00041</td></tr>
</table>
From these considerations, we conclude that for gamma rays of 500 to 1500 keV, the dose rate (μSv/h) can be obtained by multiplying the number of counts per minute (cpm) by 0.090, and the dose (μSv) can be obtained by multiplying the cumulative count by 0.00150. For dose rates of 1.0 μSv/h or less, the error will be large if it's counted only for 1 min, so the value should be obtained from the mean over a 10-minute period. Especially at low dose rates of about 0.1 μSv/h or lower, the error would be even larger.<BR>
以上の検討から、500～1500 keVのガンマ線ならば、この測定器での１分あたりのカウント数(cpm)に0.090を掛けると線量率(μSv/h)が得られ、カウント数の累計に0.00150を掛けると線量(μSv)が求められる、と結論しました。1.0 μSv/h以下の線量率では、１分だけの計数では誤差が大きくなってしまいますので、10分間の平均値から求めます。特に0.1 μSv/h程度あるいはそれ以下の低線量率では、さらに誤差が大きくなると思います。<BR>

## Firmware ファームウエア
Please find source code and its hex under the firmware folder. Hostapp is also available. <BR>
ファームウエアはfirmwareフォルダに置きました。ホスト側のアプリもあります。

## DISCLAIMER お断り
This repository is provided "AS IS", without warranty of any kind, either express or implied. Use it on your own responsibility.<BR>
このリポジトリは「そのまま」で提供されており、明示・暗示を問わず、いかなる保証もありません。ご利用はご自身のご責任でどうぞ。
