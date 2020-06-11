\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{IHCreport}[2018/05/10 v1.0 Royal IHC report class]
\RequirePackage[dutch,english]{babel}
\RequirePackage{iflang}
\RequirePackage{ifthen}
\RequirePackage{pgfkeys}
\RequirePackage{pgfopts}
\RequirePackage{etoolbox}
\RequirePackage{xparse}

% === HANDLES THE LANGUAGE ===
\DeclareOption{english}{\AtEndOfClass{\main@language{english}}}
\DeclareOption{dutch}{\AtEndOfClass{\main@language{dutch}}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrbook}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrlayer-scrpage}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{scrlayer}}

% === CLASS OPTIONS ===
\newif\ifshowunits%
\pgfkeys{
 /IHCreport/.is family,
 /IHCreport,
 businessunit/.store in=\isUnit,
 businessunit=ihc,
 headingstyle/.store in=\headingstyle,
 headingstyle=color,
 file glossary/.store in=\fglossary,
 file glossary=resources/glossary/glossary,
 showunits/.is if = showunits,
 showunits=false,
 classification/.store in=\classificationstyle,
 classification=none,
 status/.store in=\status,
 status=concept,
}

\ProcessOptions\relax

\ProcessPgfOptions{/IHCreport}

\LoadClass[10pt,%
 twoside,%
 DIV=15,%
 BCOR=15mm,%
 footinclude,%
 headinclude,%
 open=any,%
 titlepage=true,%
 appendixprefix=false,%
 chapterprefix=false,%
 numbers=noenddot,%
 captions=setting]{scrbook}
\usepackage[automark]{scrlayer-scrpage}
\usepackage{scrlayer}
\usepackage[useregional]{datetime2}
\usepackage{caption}

% === HELPERS ===
\usepackage{changepage}
\usepackage{import}

\newcommand{\newlanguagecommand}[1]{%
 \newcommand#1{%
  \@ifundefined{\string#1\languagename}
  {``No def of \texttt{\string#1} for \languagename''}
  {\@nameuse{\string#1\languagename}}%
 }%
}
\newcommand{\addtolanguagecommand}[3]{%
 \@namedef{\string#1#2}{#3}}

 % === COLORS ===

 \usepackage[svgnames, table]{xcolor}
 \definecolor{RoyalRed}{HTML}{FF0000}
 \definecolor{RoyalBlack}{HTML}{000000}
 \definecolor{RoyalGrey}{HTML}{968F85}
 \definecolor{RoyalLightGrey}{HTML}{E0E0E0}
 \definecolor{RoyalWhite}{HTML}{FFFFFF}

% === HOOKS ===
\AtEndPreamble{%
 \addbibresource{resources/report.bib}%
 \input{user.sty}%
 \makeglossaries%
 \loadglsentries{\fglossary}%
}
\AtBeginDocument{%
 \let\thedate\@date%
}

% === HYPERREF ===
\usepackage[hidelinks,
 pdfencoding=auto,
 pdfpagemode=UseOutlines,
 pdfpagelayout=TwoPageRight,
 pdfdisplaydoctitle,
 pdfstartview=Fit]{hyperref}
\usepackage{bookmark}

% === REPORT METADATA ===
\newcommand{\unitname}[1]{
 \def\@unitname{#1}
 \publishers{#1}
}
\newcommand{\unitbox}[1]{\def\@unitbox{#1}}
\newcommand{\unitadress}[1]{\def\@unitadress{#1}}
\newcommand{\unittel}[1]{\def\@unittel{#1}}
\newcommand{\unitmail}[1]{\def\@unitmail{#1}}
\newcommand{\unitsite}[1]{\def\@unitsite{#1}}

\newcommand{\client}[1]{\def\@client{#1}}
\newcommand{\externalreference}[1]{\def\@externalreference{#1}}
\newcommand{\internalreference}[1]{\def\@internalreference{#1}}
\newcommand{\version}[1]{\def\@version{#1}}

\newlanguagecommand{\tblclmwrittenby}
\addtolanguagecommand{\tblclmwrittenby}{english}{WRITTEN BY}
\addtolanguagecommand{\tblclmwrittenby}{dutch}{GESCREVEN DOOR}

\newcommand{\addauthor}[4]{
 \author{#1}
 \affil{\MakeUppercase{#4}}
 \listadd\personslist{\tblclmwrittenby}
 \listadd\personslist{#1}
 \ifstrempty{#2}{\listadd\personslist{empty}}{\listadd\personslist{#2}}
 \ifstrempty{#3}{\listadd\personslist{empty}}{\listadd\personslist{#3}}
}

\newlanguagecommand{\tblclmapprovedby}
\addtolanguagecommand{\tblclmapprovedby}{english}{APPROVED BY}
\addtolanguagecommand{\tblclmapprovedby}{dutch}{GOEDGEKEURD DOOR}

\newcommand{\addapprovedby}[3]{
 \listadd\personslist{\tblclmapprovedby}
 \listadd\personslist{#1}
 \ifstrempty{#2}{\listadd\personslist{empty}}{\listadd\personslist{#2}}
 \ifstrempty{#3}{\listadd\personslist{empty}}{\listadd\personslist{#3}}
}

\newlanguagecommand{\tblclmreviewedby}
\addtolanguagecommand{\tblclmreviewedby}{english}{REVIEWD BY}
\addtolanguagecommand{\tblclmreviewedby}{dutch}{BEOORDEELD DOOR}

\newcommand{\addreviewedby}[3]{
 \listadd\personslist{\tblclmreviewedby}
 \listadd\personslist{#1}
 \ifstrempty{#2}{\listadd\personslist{empty}}{\listadd\personslist{#2}}
 \ifstrempty{#3}{\listadd\personslist{empty}}{\listadd\personslist{#3}}
}

\unitname{IHC MTI B.V.}
\ifthenelse{\equal{\isUnit}{mti}}
{
 \unitname{IHC MTI B.V.}
 \unitbox{P.O. Box 2, 2600 MB  Delft}
 \unitadress{Delftechpark 13, 2628 XJ  Delft}
 \unittel{+31 88 015 2535}
 \unitmail{info@ihcmti.com}
 \unitsite{www.ihcmti.com}
}{}
\ifthenelse{\equal{\isUnit}{ihc}}
{
 \unitname{IHC Holland B.V.}
 \unitbox{P.O. Box 1, 2961 AW  Kinderdijk}
 \unitadress{Smitweg 6, 2961 AW  Kinderdijk}
 \unittel{+31 88 015 2535}
 \unitmail{info@royalihc.com}
 \unitsite{www.royalihc.com}
}{}
\ifthenelse{\equal{\isUnit}{medusa}}
{
 \unitname{IHC Medusa B.V.}
 \unitbox{P.O. Box 1, 2961 AW  Kinderdijk}
 \unitadress{Smitweg 6, 2961 AW  Kinderdijk}
 \unittel{+31 88 015 2535}
 \unitmail{info@royalihc.com}
 \unitsite{www.royalihc.com}
}{}

% === TABLES ===
\RequirePackage{tabu}
\usepackage{multirow}
\usepackage{longtable}
\usepackage{colortbl}

\NewDocumentCommand\RoyalCell{om}
{
  \IfValueT{#1}
  {
    \color{#1} #2
  }
  #2
  \let\RoyalCell\RoyalCella%
}
\NewDocumentCommand\RoyalCella{om}
{
  &
  \IfValueT{#1}
  {
  \color{#1}
  }
  #2
}

\NewDocumentCommand\RoyalRow{>{\SplitList{|}}m}
{
  \\ \ProcessList{#1}{\RoyalCell}
}

\NewDocumentCommand\RoyalHeader{>{\SplitList{|}}m}
{
  \ProcessList{#1}{\RoyalCell}
}


\NewDocumentEnvironment{ RoyalTable }{m}
{
\setlength{\tabulinesep}{1pt}
\setlength{\extrarowsep}{2pt}
\noindent
\taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}
\begin{longtabu}{#1}
  \rowcolor{RoyalRed} \rowfont{\sffamily\bfseries\color{RoyalWhite}}
}
{
\end{longtabu}
}

\NewDocumentEnvironment{ RoyalUM }{m}
{
\setlength{\tabulinesep}{1pt}
\setlength{\extrarowsep}{2pt}
\noindent
\taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}
\begin{longtabu} to 170mm {#1}
  \rowcolor{RoyalRed} \rowfont{\sffamily\bfseries\color{RoyalWhite}}
}
{
\end{longtabu}
}

% === GRAPHICS ===
\usepackage{graphicx}
\graphicspath{ {resources/} }
%\usepackage{minted}
%\usepackage{morewrites}
\usepackage[most, minted]{tcolorbox}
\tcbuselibrary{minted}
\usepackage{tikz}


\usetikzlibrary{calc, arrows, arrows.meta, shapes, positioning, shadows, trees, fit,automata,patterns}

%tikz controller shit
\tikzstyle{block} = [draw, fill=RoyalLightGrey, rectangle,
minimum height=3em, minimum width=6em,rounded corners=.15cm]
\tikzstyle{blockgrey} = [draw, fill=RoyalLightGrey, rectangle,
minimum height=3em, minimum width=6em,rounded corners=.15cm]
\tikzstyle{blockdashed} = [draw=RoyalRed,dashed, fill=RoyalLightGrey, rectangle,
minimum height=3em, minimum width=6em,rounded corners=.15cm]
\tikzstyle{blockdashedwhite} = [draw=RoyalRed,dashed, rectangle,
minimum height=3em, minimum width=6em,rounded corners=.15cm]
\tikzstyle{sum} = [draw, fill=RoyalRed!50, circle, node distance=2cm]
\tikzstyle{input} = [coordinate]
\tikzstyle{output} = [coordinate]
\tikzstyle{pinstyle} = [pin edge={to-,thin,RoyalBlack}]

% === FONT ===
\ifthenelse{\equal{\status}{concept}}{}{%
\usepackage[type1, default]{lato}
\usepackage[type1, black]{raleway}
}
\usepackage{mdsymbol}
%\usepackage{amsmath,amssymb}
%\usepackage[no-math]{fontspec}
%\usepackage[T1]{fontenc}
%\usepackage[utf8]{inputenc}
\usepackage{bm}
\usepackage[utf8]{luainputenc}
\usepackage{mathrsfs}
\let\Re\undefined%
\let\Im\undefined%
\DeclareMathOperator{\Re}{\mathscr{R}}
\DeclareMathOperator{\Im}{\mathscr{I}}
\DeclareMathOperator{\Eu}{\mathscr{E}}
% === HEADINGS ===
\usepackage{stringstrings}
\newcommand{\colorheading}{\ifthenelse{\equal{\headingstyle}{color}}{\color{RoyalRed}}{\normalcolor}}

\setkomafont{disposition}{\color{RoyalBlack}\sffamily\bfseries}
\setkomafont{chapter}{\huge}
\setkomafont{chapterprefix}{\huge\color{RoyalRed}}

% Sections to uppercase. See https://tex.stackexchange.com/questions/33148/setkomafont-with-uppercase-that-works-robustly/33215#33215
\renewcommand*{\chapterformat}{\chapappifchapterprefix{\nobreakspace}\thechapter\IfUsePrefixLine{}{}}

\let\scr@chapter\chapter%
\def\chapter{\@ifstar\seamus@chapter\seamus@chapter}
\def\seamus@chapters#1{\scr@chapter*{#1}}
\def\seamus@chapter{\@dblarg{\seamus@chapter@}}
\def\seamus@chapter@[#1]#2{%
 \scr@chapter[#1]{\MakeUppercase{#2}}}

\renewcommand*{\sectionformat}{\colorheading\thesection\enskip}
\let\scr@section\section
\def\section{\@ifstar\seamus@sections\seamus@section}
\def\seamus@sections#1{\scr@section*{\MakeUppercase{#1}}}
\def\seamus@section{\@dblarg{\seamus@section@}}
\def\seamus@section@[#1]#2{%
 \scr@section[#1]{\MakeUppercase{#2}}}

\renewcommand*{\subsectionformat}{\colorheading\thesubsection\enskip}
\let\scr@subsection\subsection
\def\subsection{\@ifstar\seamus@subsections\seamus@subsection}
\def\seamus@subsections#1{\scr@subsection*{\MakeUppercase{#1}}}
\def\seamus@subsection{\@dblarg{\seamus@subsection@}}
\def\seamus@subsection@[#1]#2{%
 \scr@subsection[#1]{\MakeUppercase{#2}}}

\setkomafont{subsubsection}{\colorheading}
\let\scr@subsubsection\subsubsection
\def\subsubsection{\@ifstar\seamus@subsubsection\seamus@subsubsection}
\def\seamus@subsubsections#1{\scr@subsubsection*{\MakeUppercase{#1}}}
\def\seamus@subsubsection{\@dblarg{\seamus@subsubsection@}}
\def\seamus@subsubsection@[#1]#2{%
 \scr@subsubsection[#1]{\MakeUppercase{#2}}}

\renewcommand*{\othersectionlevelsformat}[3]{\colorheading#3\enskip}

\newlanguagecommand{\summarycaption}
\addtolanguagecommand{\summarycaption}{english}{SUMMARY}
\addtolanguagecommand{\summarycaption}{dutch}{SAMENVATTING}
\newcommand{\summary}{\chapter*{\summarycaption}\label{chap:summary}}

\newlanguagecommand{\introductioncaption}
\addtolanguagecommand{\introductioncaption}{english}{INTRODUCTION}
\addtolanguagecommand{\introductioncaption}{dutch}{INLEIDING}
\newcommand{\introduction}{\chapter{\introductioncaption}\label{chap:introduction}}

\newlanguagecommand{\conclusioncaption}
\addtolanguagecommand{\conclusioncaption}{english}{CONCLUSION}
\addtolanguagecommand{\conclusioncaption}{dutch}{CONCLUSIE}
\newcommand{\conclusion}{\chapter{\conclusioncaption}\label{chap:conclusion}}

% === APPENDIX ===
\usepackage[page,titletoc,title,toc,header]{appendix}
\addto\captionsdutch{\renewcommand{\appendixname}{BIJLAGE}}
\addto\captionsdutch{\renewcommand{\appendixpagename}{\sffamily\bfseries\Huge BIJLAGES}}
\addto\captionsenglish{\renewcommand{\appendixpagename}{\sffamily\bfseries\Huge APPENDICES}}
\addto\captionsdutch{\renewcommand{\appendixtocname}{BIJLAGES}}
\renewcommand{\appendixtocname}{APPENDICES}

% === LAYOUT ===
\usepackage{lipsum}
\usepackage{ragged2e}
% \usepackage[
%  a4paper,
%  tmargin=5mm,
%  textwidth=155mm,
%  textheight=250mm,
%  heightrounded,
%  twoside,
%  bindingoffset=20mm,
%  includeheadfoot,
%  headheight=10mm,
%  headsep=8mm,
%  foot=15mm
% ]{geometry}

\setkomafont{captionlabel}{\sffamily\bfseries}
\addtokomafont{caption}{\sffamily\bfseries\MakeUppercase}
%\setkomafont{descriptionlabel}{\bfseries}

\setlength\footheight{20pt}
\ohead[]{}
\chead[]{}
\ihead[]{}

\DeclareLayer[
 background,
 topmargin,
 addheight=\topskip,
 mode=picture,
 oddpage,
 contents={
   \if@mainmatter
    \begin{tikzpicture}[overlay,remember picture]
     \node[fill=RoyalGrey, anchor=north east, minimum height=\LenToUnit{\layerheight}, inner sep=5mm] at (current page.north east) {\sffamily\bfseries\Large\color{RoyalWhite}\MakeUppercase{\leftmark}};
    \end{tikzpicture}
   \else \fi
  }
]{scrheadings.head.odd.background}
\DeclareLayer[
 background,
 topmargin,
 addheight=\topskip,
 mode=picture,
 evenpage,
 contents={
   \if@mainmatter
    \begin{tikzpicture}[overlay,remember picture]
     \node[fill=RoyalGrey, anchor=north west, minimum height=\LenToUnit{\layerheight}, inner sep=5mm] at (current page.north west) {\sffamily\bfseries\Large\color{RoyalWhite}\MakeUppercase{\@unitname}};
    \end{tikzpicture}
   \else \fi
  }
]{scrheadings.head.even.background}

\AddLayersAtBeginOfPageStyle{scrheadings}{%
 scrheadings.head.odd.background,%
 scrheadings.head.even.background,%
}

\setkomafont{pagenumber}{\sffamily\bfseries\color{RoyalRed}\Large|\color{RoyalBlack}}
\newkomafont{chapternumber}{\sffamily\fontsize{42pt}{40pt}\selectfont\color{RoyalGrey}}

\tikzset{
 headings/base/.style = {
   outer sep = 0pt,
   inner sep = 2pt,
  },
 headings/chapterbackground/.style = {
   headings/base,
   font = \usekomafont{chapter},
  },
 headings/chapapp/.style = {
   headings/base,
   font = \usekomafont{chapterprefix},
  },
 headings/chapternumber/.style= {
   headings/base,
   text = RoyalGrey,
   fill = RoyalWhite,
   font = \usekomafont{chapternumber},
  },
}
\usepackage{xstring}

\renewcommand\chapterlinesformat[3]{%
     \ifnumodd{\thepage}{%
      \begin{tikzpicture}[overlay,remember picture]%
       \node[headings/chapterbackground, anchor=south east, yshift=-45mm, xshift=-20mm] at (current page.north east) (ch) {#3};%
       \node[headings/chapternumber, anchor=south west] at (ch.south east) (chnr) {#2};%
       \node[headings/chapapp, anchor=south east] at (ch.north east) (chapp) {\MakeUppercase{\chapapp}};%
      \end{tikzpicture}%
     }{%
      \begin{tikzpicture}[overlay,remember picture]%
       \node[headings/chapterbackground, anchor=south west, yshift=-45mm, xshift=20mm] at (current page.north west) (ch) {#3};%
       \node[headings/chapternumber, anchor=south east] at (ch.south west) (chnr) {#2};%
       \node[headings/chapapp, anchor=south west] at (ch.north west) (chapp) {\MakeUppercase{\chapapp}};%
      \end{tikzpicture}%
     }%
}

% === DOCUMENT STRUCTURE ===
\newif\ifmcover
\newif\ifmtitle
\newif\ifmquality
\newif\ifmdisclaimer
\pgfkeys{
 front/.is family, /front,
 fedefault/.style =
  {page style=frontmatter, no cover=false, no title=false, no qualitycontrol=false, no disclaimer=false},
 no cover/.is if = mcover,
 no title/.is if = mtitle,
 no qualitycontrol/.is if = mquality,
 no disclaimer/.is if = mdisclaimer,
 page style/.estore in = \festyle,
}

\NewDocumentEnvironment{front} { o }
{%
\frontmatter%
\maketitle
\setcounter{page}{1}%
\pagenumbering{Roman}%
\makequalitycontrol%
\makedisclaimer%
}
{%
\tableofcontents%
\glsresetall%
}

\NewDocumentEnvironment{main}{}
{%
\mainmatter%
\setcounter{page}{1}%
\glsresetall%
}
{%
\label{lastoffront}%
}

\NewDocumentEnvironment{app}{}
{%
\appendix
\setcounter{figure}{0}\renewcommand{\thefigure}{\thechapter.\arabic{figure}}%
\setcounter{table}{0}\renewcommand{\thetable}{\thechapter.\arabic{table}}%
\appendixpage%
}{}

\newif\ifmallsources
\newif\ifmallglossaries
\newif\ifmbackcover
\pgfkeys{
 back/.is family, /back,
 bedefault/.style =
  {no backcover=false, all sources=false, all glossaries=false},
 no backcover/.is if = mbackcover,
 all sources/.is if = mallsources,
 all glossaries/.is if = mallglossaries,
}
\NewDocumentEnvironment{back}{ o }
{%
\pgfkeys{/back, bedefault, #1}%
\cleardoubleoddstandardpage,%
\ifmallsources\nocite{*}\else\fi%
\ifmallglossaries\glsaddall\else\fi%
}{}

% === QUALITY CONTROL PAGE ===
\usepackage{intcalc}
\usepackage{array}

\newlanguagecommand{\qualitycontrolcaption}
\addtolanguagecommand{\qualitycontrolcaption}{english}{Quality control}
\addtolanguagecommand{\qualitycontrolcaption}{dutch}{Kwaliteitscontrole}

\newlanguagecommand{\qualitycontroltxt}
\addtolanguagecommand{\qualitycontroltxt}{english}{%
\noindent This report has been reviewed and approved in accordance with the policies of \@unitname \\%
}
\addtolanguagecommand{\qualitycontroltxt}{dutch}{%
\noindent Dit rapport is conform het beleid van \@unitname\ gecontrolleerd en goedgekeurd. \\%
}

\newlanguagecommand{\tblheadername}
\addtolanguagecommand{\tblheadername}{english}{Name}
\addtolanguagecommand{\tblheadername}{dutch}{Naam}

\newlanguagecommand{\tblheaderdate}
\addtolanguagecommand{\tblheaderdate}{english}{Date}
\addtolanguagecommand{\tblheaderdate}{dutch}{Datum}

\newlanguagecommand{\tblheadersignature}
\addtolanguagecommand{\tblheadersignature}{english}{Signature}
\addtolanguagecommand{\tblheadersignature}{dutch}{Handtekening}

\newcommand{\makequalitycontrol}{%
\vspace*{10mm}%
\section*{\qualitycontrolcaption}%
\qualitycontroltxt%
\newcounter{personscounter}%
\setcounter{personscounter}{0}%
\newcommand{\personrowdo}[1]{%
\stepcounter{personscounter}%
\ifnumequal{\intcalcMod{\value{personscounter}}{4}}{1}{##1 &}{}%
\ifnumequal{\intcalcMod{\value{personscounter}}{4}}{2}{##1 &}{}%
\ifnumequal{\intcalcMod{\value{personscounter}}{4}}{3}{\ifstrequal{##1}{empty}{}{##1}&}{}%
\ifnumequal{\intcalcMod{\value{personscounter}}{4}}{0}{\ifstrequal{##1}{empty}{\raisebox{10mm}{}}{\includegraphics[height=10mm]{##1}} \\ \hline}{}}%
\newcommand*{\personrow}{\forlistloop{\personrowdo}{\personslist}}%
\noindent%
\extrarowsep=1mm%
{\small%
\begin{tabu}{|>{\bfseries}l|l|l|>{\centering\arraybackslash}X[c,m]|}%
\cline{2-4}%
\rowfont \bfseries%
\multicolumn{1}{c|}{} & \tblheadername & \tblheaderdate & \tblheadersignature \\ \tabucline{1-4}%
\personrow%
\end{tabu}%
\extrarowsep=0mm}%
}

% === DISCLAIMER PAGE ===
\newlanguagecommand{\disclaimercaption}
\addtolanguagecommand{\disclaimercaption}{english}{Disclaimer}
\addtolanguagecommand{\disclaimercaption}{dutch}{Disclaimer}

\newlanguagecommand{\disclaimertxt}
\addtolanguagecommand{\disclaimertxt}{english}{%
This document has been prepared by \@unitname\ for \@client. The opinions and%
information in this report are entirely those of \@unitname, based on data and%
assumptions as reported throughout the text and upon information and data%
obtained from sources which \@unitname\ believes to be reliable.\newline%
While \@unitname\ has taken all reasonable care to ensure that the facts and%
opinions expressed in this document are accurate, it makes no guarantee to any%
person, organization or company, representation or warranty, express or implied%
as to fairness, accuracy and liability for any loss howsoever, arising directly%
or indirectly from its use or contents.\newline%
This document is intended for use by professionals or institutions. This%
document remains the property of \@unitname. All rights reserved. This document%
or any part thereof may not be made public or disclosed, copied or otherwise%
reproduced or used in any form or by any means, without prior permission in%
writing from \@unitname. \newline%
\begin{center}%
This document is dated \thedate%
\end{center}%
}
\addtolanguagecommand{\disclaimertxt}{dutch}{%
Dit document is geschreven door \@unitname\ voor \@client. Meningen%
en informatie in dit rapport zijn in het geheel van \@unitname\ en gebaseerd%
op onderliggende gegevens en aannames, zoals aangegeven in de tekst. Gegevens%
welke door \@unitname\ zelf zijn verkregen worden als betrouwbaar gezien.\newline%
\@unitname\ heeft alle zorg gedragen om feiten en meningen correct en%
nauwkeurig te documenteren. Er kan geen zekerheidsstelling, zowel expliciet als%
impliciet aan een persoon, organisatie, bedrijf, garantie of vertegenwoordiger%
hiervan gegeven worden. \@unitname\ accepteert geen aansprakelijkheid voor elk%
direct en indirect geleden verlies, voortvloeiend uit dit document en haar%
inhoud.\newline%
Dit document is bedoeld voor professionals en instituten. Dit document blijft%
eigendom van \@unitname. Alle rechten voorbehouden. Dit document of een%
gedeelte hiervan mag daarom niet gepubliceerd, vrijgegeven, gekopieerd of op%
enige andere wijze gereproduceerd, of gebruikt worden, in elke vorm of%
mogelijkheid, zonder van te voren verkregen schriftelijke toestemming door%
\@unitname. \newline%
\begin{center}%
Dit document is gepubliceerd op \thedate%
mogelijkheid, zonder van te voren verkregen schriftelijke toestemming door%
\@unitname. \newline%
\begin{center}%
Dit document is gepubliceerd op \thedate%
\end{center}%
}%

\newcommand{\makedisclaimer}{%
\section*{\disclaimercaption}%
{\parindent0pt\disclaimertxt}%
}%

% === CLASSIFICATION ===
\newkomafont{watermarkfont}{\sffamily\fontsize{70pt}{70pt}\selectfont\sffamily\bfseries}
\tikzset{
 watermark/base/.style = {
   outer sep = 0pt,
   inner sep = 5pt,
   rotate=45,
   font = \usekomafont{watermarkfont},
   text = RoyalLightGrey
  },
}

\newlanguagecommand{\wmconfidential}
\addtolanguagecommand{\wmconfidential}{english}{CONFIDENTIAL}
\addtolanguagecommand{\wmconfidential}{dutch}{VERTROUWELIJK}

\DeclareNewLayer[
 background,
 textarea,
 mode=picture,
 contents={
   \putC{\begin{tikzpicture}[overlay,remember picture]
     \node[watermark/base] {\wmconfidential};
    \end{tikzpicture}}
  }
]{confidentialwatermark}
\IfEq{\classificationstyle}{confidential}{\AddLayersToPageStyle{@everystyle@}{confidentialwatermark}}{}

\newlanguagecommand{\wmsecret}
\addtolanguagecommand{\wmsecret}{english}{SECRET}
\addtolanguagecommand{\wmsecret}{dutch}{GEHEIM}

\DeclareNewLayer[
 background,
 textarea,
 mode=picture,
 contents={
   \putC{\begin{tikzpicture}[overlay,remember picture]
     \node[watermark/base] {\wmsecret};
    \end{tikzpicture}}
  }
]{secretwatermark}
\IfEq{\classificationstyle}{secret}{\AddLayersToPageStyle{@everystyle@}{secretwatermark}}{}

% === TITLEPAGE ===
\usepackage{authblk}

\DeclareNewLayer[%
 foreground,%
 foot,%
 addheight=\topskip,%
 mode=picture,%
 contents={%
   \begin{tikzpicture}[overlay,remember picture]%
    \node[inner sep=0pt, outer sep=0pt, anchor=south west, xshift=34mm, yshift=18.5mm] at (current page.south west) {\includegraphics[height=16mm]{footer.png}};%
   \end{tikzpicture}%
  }%
]{titlepagelogo}
\DeclarePageStyleByLayers{titlepage}{titlepagelogo}
\newlanguagecommand{\titlepublishedby}
\addtolanguagecommand{\titlepublishedby}{english}{Published by:~}
\addtolanguagecommand{\titlepublishedby}{dutch}{Uitgegeven door:~}

\setcounter{Maxaffil}{1}
\renewcommand\Affilfont{\sffamily\footnotesize}

\setkomafont{title}{\sffamily\bfseries\Huge\color{RoyalRed}}
\setkomafont{subtitle}{\sffamily\bfseries\huge\color{RoyalBlack}}
\newkomafont{subsubtitle}{\sffamily\bfseries\huge\color{RoyalGrey}}
\setkomafont{publishers}{\sffamily\large\color{RoyalBlack}}

\tikzset{
 titlenode/base/.style = {
   outer sep = 0pt,
   inner sep = 1pt,
   anchor = north east,
   fill = RoyalWhite,
  },
}
\renewcommand*{\maketitle}{%
 \begin{titlepage}%
  \ifmcover\else\makefrontcover\fi%
  \thispagestyle{titlepage}%
  \global\@topnum=\z@\setparsizes{\z@}{\z@}{\z@\@plus 1fil}%
  \par@updaterelative%
  \begin{tikzpicture}[overlay,remember picture]%
   \node[titlenode/base, yshift=-25mm, xshift=-\coverpagerightmargin] at (current page.north east) (titlenode) {\usekomafont{title}\MakeUppercase{\@title}};%
   \ifx \@subtitle %TODO fix no subtitle defined%
    \node[titlenode/base] at (titlenode.south east) (subtitlenode) {\usekomafont{subtitle}\MakeUppercase{\@unitname}};%
   \else%
    \node[titlenode/base, align=left] at (titlenode.south east) (subtitlenode) {\usekomafont{subtitle}\@subtitle};%
    \node[titlenode/base] at (subtitlenode.south east) (subsubtitlenode) {\usekomafont{subsubtitle}\MakeUppercase{\@unitname}};%
   \fi%
   \node[anchor=center] at (0.5*\paperwidth-\coverpageleftmargin,-0.25*\paperheight) (authornode) {%
    {\usekomafont{author}{\lineskip .5em%
       \begin{tabular}[t]{c}%
        \@author%
       \end{tabular}\par}}%
   };%
   \node[anchor=north, inner sep=0pt, outer sep=1em] at (authornode.south) (datenode) {\usekomafont{date}\@date};%
   \node[anchor=south east, xshift=-\coverpagerightmargin, yshift=50mm, inner sep=0pt, outer sep=0pt] at (current page.south east) (addrnode) {%
    \begin{tabu}{r}%
     \@unitname                         \\%
     \@unitbox                          \\%
     \@unitadress                       \\%
     S: \href{\@unitsite}{\@unitmail}   \\%
     T: \href{tel:\@unittel}{\@unittel} \\%
     M: \href{mailto:\@unitmail}{\@unitmail}%
    \end{tabu}%
   };%
   \node[anchor=north west, inner sep=0pt, outer sep=0pt, xshift=34mm] at (current page.south west |- addrnode.north) {%
    \begin{tabu}{l l}%
     Client             & \@client             \\%
     External reference & \@externalreference  \\%
     Internal reference & \@internalreference  \\%
     Version            & \@version            \\%
     Status             & \status              \\%
     Classification     & \classificationstyle%
    \end{tabu}%
   };%
  \end{tikzpicture}%
  \cleardoublepage%
 \end{titlepage}%
}
%TODO fix no subtitle defined%
% === COVERPAGE FRONT ===
\tikzset{%
 covernode/base/.style = {%
   outer sep = 0pt,%
   inner sep = 1pt,%
   anchor = north west,%
   fill = RoyalWhite,%
  },%
}%
\setkomafont{subject}{\color{RoyalBlack}\large\raggedright}%
\NewDocumentCommand{\makefrontcover}{}{\thispagestyle{titlepage}%
 \begin{tikzpicture}[overlay,remember picture]%
    \node[covernode/base, yshift=-16mm, xshift=34mm] at (current page.north west) (titlebackground) { \begin{tikzpicture}%
        \node[covernode/base] (titlenode) {\usekomafont{title}\MakeUppercase{\@title}};%
        \node[covernode/base,align=left] at (titlenode.south west) (subtitlenode) {{\usekomafont{subtitle}DESIGN OF AN AUTONOMOUS}};%
        \node[covernode/base] at (subtitlenode.south west) (subsubtitlenode) {\usekomafont{subsubtitle}DREDGE BOT CONTROLLER};%
      \end{tikzpicture}};%
    \node[inner sep=0pt, outer sep=0pt, anchor=north west, yshift=-4mm] at (titlebackground.south west) (imgnode) {\includegraphics[width=\paperwidth-80mm]{front.pdf}};%
        \node[covernode/base, anchor=south east, inner sep = 5pt, xshift=16mm, yshift=-16mm] at (imgnode.south east) (subjectnode) {\parbox[b]{64mm}{\usekomafont{subject}\@subject}};%
 \end{tikzpicture}%
 \cleardoublepage}



% === BIBLIOGRAPHY ===
\usepackage[backend=biber, bibencoding=utf8, style=ieee, sorting=ynt]{biblatex}
\newcommand{\citet}[1]{\citeauthor{#1}~\cite{#1}}

% === UNITS ===
\newlanguagecommand{\rangephrase}
\addtolanguagecommand{\rangephrase}{english}{~to~}
\addtolanguagecommand{\rangephrase}{dutch}{~tot~}

\usepackage[%
 separate-uncertainty=true,%
 per-mode = symbol-or-fraction,%
 list-units = brackets,%
 range-units = brackets,%
 round-mode=places,%
 round-precision=1,%
 round-integer-to-decimal=true,%
 multi-part-units=single,%
 binary-units]{siunitx}
\sisetup{range-phrase=\rangephrase}

\input{resources/customunits.def}

% === GLOSSARY ===
%\usepackage[acronym,xindy,section=chapter]{glossaries}
\usepackage[automake=immediate,acronym,xindy,docdef=atom,section=chapter,symbols]{glossaries-extra}
\setabbreviationstyle[acronym]{long-short}
\usepackage{datatool}
%\glsdisablehyper
\usepackage[dvipdfmx]{pdfcomment}

\glsaddkey{unit}{\glsentrytext{\glslabel}}{\glsentryunit}{\GLsentryunit}{\glsunit}{\Glsunit}{\GLSunit}
\newglossary[slg]{symbols}{sys}{syo}{Symbols}


%\defglsentryfmt[main]{\pdfmarkupcomment[author=Spijker,subject=glossary,color={1 0 0},opacity=0.33]{\glsgenentryfmt}{\glsentrydesc{\glslabel}}}
%\defglsentryfmt[\acronymtype]{\pdfmarkupcomment[author=Spijker,subject=acronym,color={0 1 0},opacity=0.33]{\mbox{\glsgenentryfmt}}{\glsentrydesc{\glslabel}}}

\newlanguagecommand{\unitgivenin}
\addtolanguagecommand{\unitgivenin}{english}{Units are given in:}
\addtolanguagecommand{\unitgivenin}{dutch}{Eenheden zijn opgegeven in:}
%\defglsentryfmt[symbolslist]{\pdfmarkupcomment[author=Spijker,subject=symbol,color={0 0 1},opacity=0.33]{\expandonce\texorpdfstring{\ensuremath{\glsgenentryfmt}}{\glslabel}}{\glsentrydesc{\glslabel} \unitgivenin\ \glsentryunit{\glslabel}}}




\newlanguagecommand{\signheader}
\addtolanguagecommand{\signheader}{english}{SIGN}
\addtolanguagecommand{\signheader}{dutch}{TEKEN}

\newlanguagecommand{\keyheader}
\addtolanguagecommand{\keyheader}{english}{KEY}
\addtolanguagecommand{\keyheader}{dutch}{SLEUTEL}

\newlanguagecommand{\descriptionheader}
\addtolanguagecommand{\descriptionheader}{english}{DESCRIPTION}
\addtolanguagecommand{\descriptionheader}{dutch}{BESCHRIJVING}

\newlanguagecommand{\unitheader}
\addtolanguagecommand{\unitheader}{english}{UNIT}
\addtolanguagecommand{\unitheader}{dutch}{EENHEID}

\newlanguagecommand{\pageheader}
\addtolanguagecommand{\pageheader}{english}{PAGE}
\addtolanguagecommand{\pageheader}{dutch}{PAGINA}

\newlanguagecommand{\symbolslisttitle}
\addtolanguagecommand{\symbolslisttitle}{english}{NOMENCLATURE}
\addtolanguagecommand{\symbolslisttitle}{dutch}{NOMENCLATURE}

\newlanguagecommand{\glostitle}
\addtolanguagecommand{\glostitle}{english}{GLOSSARY}
\addtolanguagecommand{\glostitle}{dutch}{WOORDENLIJST}

\newlanguagecommand{\acrtitle}
\addtolanguagecommand{\acrtitle}{english}{ACRONYMS}
\addtolanguagecommand{\acrtitle}{dutch}{ACRONIEMEN}


\newglossarystyle{symbolsstyle}{%
 \renewenvironment{theglossary}%
 {%
  \setlength{\tabulinesep}{4mm}%
  \setlength{\extrarowsep}{2mm}%
  \noindent%
  \taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}%
  \begin{longtabu}{X[1,c,m] X[6,l,m] X[1.5,c,m] X[2,r,m]}%
   }%
   {%
  \end{longtabu}%
 }%
 \renewcommand*{\glossaryheader}{%
  \rowcolor{RoyalRed} \rowfont{\sffamily\bfseries}%
  \color{RoyalWhite} \signheader & \color{RoyalWhite} \descriptionheader & \color{RoyalWhite} \unitheader & \color{RoyalWhite} \pageheader \\\endhead%
 }%
 \renewcommand*{\glossentry}[2]{%
  \color{black}\ensuremath{\glossentryname{##1}} & \glossentrydesc{##1} & \ensuremath{\si{\glsentryunit{##1}}} & ##2 \\%
 }%
 \renewcommand*{\glsgroupheading}[1]{}%
 \renewcommand*{\glsgroupskip}{}%
}

\newglossarystyle{ihclongstyle}{%
 \renewenvironment{theglossary}%
 {%
 \setlength{\tabulinesep}{4mm}%
 \setlength{\extrarowsep}{2mm}%
 \noindent%
  \taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}%
  \begin{longtabu}{X[3,l,m] X[5,l,m] X[1,l,m]}%
   }%
   {%
  \end{longtabu}%
 }%
 \renewcommand*{\glossaryheader}{%
  \rowcolor{RoyalRed} \rowfont{\sffamily\bfseries}%
  \color{RoyalWhite} \keyheader & \color{RoyalWhite} \descriptionheader & \color{RoyalWhite} \pageheader \\\endhead%
 }%
 \renewcommand*{\glossentry}[2]{%
  \color{black}\sffamily\bfseries \glossentryname{##1} & \glossentrydesc{##1} & ##2 \\%
 }%
 \renewcommand*{\glsgroupheading}[1]{}%
 \renewcommand*{\glsgroupskip}{}%
}

\newglossarystyle{ihclongacrostyle}{%
 \renewenvironment{theglossary}%
 {%
 \setlength{\tabulinesep}{4mm}%
 \setlength{\extrarowsep}{2mm}%
 \noindent%
  \taburowcolors[2] 2{RoyalWhite .. RoyalLightGrey}%
  \begin{longtabu}{X[1,l,m] X[6,l,m] X[1,c,m]}%
   }%
   {%
  \end{longtabu}%
 }%
 \renewcommand*{\glossaryheader}{%
  \rowcolor{RoyalRed} \rowfont{\sffamily\bfseries}%
  \color{RoyalWhite} \keyheader & \color{RoyalWhite} \descriptionheader & \color{RoyalWhite} \pageheader \\\endhead%
 }%
 \renewcommand*{\glossentry}[2]{%
  \color{black}\sffamily\bfseries \glossentryname{##1} & \glossentrydesc{##1} & ##2 \\%
 }%
 \renewcommand*{\glsgroupheading}[1]{}%
 \renewcommand*{\glsgroupskip}{}%
}
\renewcommand*{\glsclearpage}{\clearpage}
\renewcommand*{\printglossaries}{%
 \label{chap:glossaries}%
 \printglossary[type=symbols,style=symbolsstyle,title=\symbolslisttitle]%
 \printglossary[type=main,style=ihclongstyle, title=\glostitle]%
 \printglossary[type=\acronymtype,style=ihclongacrostyle, title=\acrtitle]%
}


% === MATH ===
\usepackage{mathtools}
\usepackage{esvect}
\usepackage{physics}

\tcolorboxenvironment{equation}{%
 blankest, left=5mm,%
 before skip=10pt, after skip=10pt,%
 borderline west={2mm}{0pt}{RoyalRed},%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 boxsep=5mm%
}

\tcolorboxenvironment{multiline}{%
 blankest, left=5mm,%
 before skip=10pt, after skip=10pt,%
 borderline west={2mm}{0pt}{RoyalRed},%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 boxsep=5mm%
}

% === FIGURE ===
\newlanguagecommand{\rfsource}
\addtolanguagecommand{\rfsource}{english}{Source}
\addtolanguagecommand{\rfsource}{dutch}{Bron}

\addtokomafont{captionlabel}{\sffamily\bfseries}
\addto\captionsenglish{\renewcommand{\figurename}{FIGURE}}
\addto\captionsdutch{\renewcommand{\figurename}{FIGUUR}}
\addto\captionsenglish{\renewcommand{\tablename}{TABLE}}
\addto\captionsdutch{\renewcommand{\tablename}{TABEL}}

%TODO check if label can be offset when needed
\newtcolorbox[use counter*=figure]{RoyalFigure}[2][]{%
enhanced,%
 blankest,%
 center,%
 float=#1,%
 capture=hbox,%
 lowerbox=invisible,%
 boxrule=0pt,%
 halign title=flush right,%
 remember,%
 check odd page=true,%
 finish={%
   \ifoddpage%
    \node[fill=RoyalWhite, anchor=north east, draw=RoyalWhite, thick] at ([xshift=0mm]$(current page.east |- frame.south) $) {\usekomafont{captionlabel}\figurename~\thetcbcounter:~#2};%
   \else%
   \node[fill=RoyalWhite, anchor=north east, draw=RoyalWhite, thick] at ([xshift=-1.0in-8mm]$(current page.east |- frame.south) $) {\usekomafont{captionlabel}\figurename~\thetcbcounter:~#2};%
  }%
}

% === LISTINGS ===
\newtcbinputlisting[auto counter, number within=chapter]{RoyalFileListing}[4][]{%
 blankest,%
 listing engine=minted,%
 minted language=#2,%
 minted style=colorful,%
 listing file={#3},%
 listing only,%
 before skip=10pt,%
 after skip=10pt,%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 lowerbox=invisible,%
 toptitle=5pt,%
 lefttitle=5pt,%
 title={LISTING~\thetcbcounter:~\MakeUppercase{#4}},%
 fonttitle=\usekomafont{captionlabel}\color{RoyalWhite},%
 colbacktitle=RoyalGrey,%
 breakable,%
 #1%
}

\newtcblisting[auto counter, number within=chapter]{RoyalListing}[2][]{%
 blankest,%
 listing engine=minted,%
 before skip=10pt,%
 after skip=10pt,%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 lowerbox=invisible,%
 toptitle=5pt,%
 lefttitle=5pt,%
 title={LISTING~\thetcbcounter:~\MakeUppercase{#2}},%
 fonttitle=\usekomafont{captionlabel}\color{RoyalWhite},%
 colbacktitle=RoyalGrey,%
 #1%
}

% === ALGORITHMS ===

\usepackage{algpseudocode}

\newtcolorbox[auto counter, number within=chapter]{RoyalAlgorithm}[2][]{%
 blankest, left=5mm,%
 before skip=10pt, after skip=10pt,%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 boxsep=5mm,%
 title={LISTING~\thetcbcounter:~\MakeUppercase{#2}},%
 fonttitle=\usekomafont{captionlabel}\color{RoyalWhite},%
 colbacktitle=RoyalGrey,%
 #1%
}

% === NOTES ===
\newtcolorbox[auto counter, number within=chapter]{RoyalNote}[2][]{%
 blankest,%
 before skip=10pt,%
 after skip=10pt,%
 boxsep=2mm,%
 top=1mm,%
 colback=RoyalLightGrey,%
 enhanced jigsaw,%
 toptitle=5pt,%
 lefttitle=5pt,%
 lowerbox=ignored,%
 title={NOTE~\thetcbcounter:~\MakeUppercase{#2}},%
 fonttitle=\usekomafont{captionlabel}\color{RoyalWhite},%
 colbacktitle=RoyalGrey,%
 #1%
}

\usepackage{csquotes}
\usepackage{scrhack}
