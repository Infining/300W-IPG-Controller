
%% Create the overall window w/tabs
close all
%Creating the window that everything resides in
f = figure('Position',[300,200,1500,700]);
f.ToolBar = 'none'; f.MenuBar = 'none'; f.NumberTitle = 'off';
f.Name = '300W IPG Laser Controller: V1.0';
obj = GUI_Ctrl();%create instance of class that builds buttons and handles 
%their functions

%Creating the parent for the tabs
panels = uix.TabPanel('Parent',f);

%Creating the 'Main' tab 
MainH = uix.HBoxFlex('Parent',panels);
space = 3;
set(MainH,'Spacing',space,'BackgroundColor','k');
% set(Main,'Widths',[-(1500-200),-200],'Spacing',3);% For main horizontal boxes
%Creating the 'Errors & Warnings tab
Error = uix.VBox('Parent',panels);
%titling, stting the font, font size, widths, and colors for the tabs
panels.TabTitles = {'Main','Errors & Warnings'};
panels.FontSize = 12;
panels.FontName = 'Times New Roman';
panels.TabWidth = 130;
Error.BackgroundColor = [229,255,204]./255;
colorMain = [204,255,255]./255;
% Main.BackgroundColor = colorMain;
%Note on the color: This is the color the tabs would be if other structures
%were not placed on top of them and covered it up. Really dont need to
%specify this unless we change the layout of the tabs

%% Populating the 'Main' tab

% Creating the Status box/panel
%The 'Main' tab was generated as a Horizontal box (or HBox) this uses the
%GUI Layout toolbox. This allows the 'Main' tab to be partitioned into
%columns. Where the number of columns is determined by the number of
%children 'Main' has.

LeftV = uix.VBoxFlex('Parent',MainH);%Left is the 1st Child of Main. Which 
%means it will be the left most column in the 'Main' tab (and the only
%column unless we add more Children (which we will)). We made this a
%Vertical Box (or VBox) which lets us break Left into rows (depending on
%the number of Children Left has)
set(LeftV,'Spacing',space,'BackgroundColor','k');

%%%%%%%%%%%%%%%%% Create the Status Panel in 'Main' Tab %%%%%%%%%%%%%%%%%%%

StatusPan = uix.Panel('Parent',LeftV,...
    'BackgroundColor',colorMain, 'Title','Status Area','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');% 

topStatusColor = StatusPan.BackgroundColor;

statusLvl_0V = uix.VBox('Parent',StatusPan,'BackgroundColor',topStatusColor);
%statusLvl_0: 1st and only Child of StatusPan. Allows us to build rows 
%inside of the panel. Top most row will be for stauts lights
set(statusLvl_0V,'Spacing',space-3,'BackgroundColor','k');


%%%%%%% Create the top row of Status Lights %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

statusLightsH = uix.HButtonBox('Parent',statusLvl_0V,...
    'HorizontalAlignment','center','VerticalAlignment','top');
set(statusLightsH,'ButtonSize',[160 25],'Spacing',5);
String = {'connected_SL::Laser Connected: No', 'lock1_SL::Interlock #1: Open',...
    'lock2_SL::Interlock #2: Open','ready_SL::Laser Ready: No',...
    'guideLaser_SL::Guide Laser: Off','emission_SL::Laser Emission: Off'}; 

obj = obj.build_uicontrols(statusLightsH,String,'BackgroundColor','r','Enable','inactive');

%%%%%%%% Create region in status panel for graphical & numerical status of the laser%%%%%%%%%%% 

statusLvl_1H = uix.HBoxFlex('Parent',statusLvl_0V,'BackgroundColor',topStatusColor);
%statusLvl1: 2nd and last Child of statusLvl_0. Allows us to build columns 
%inside of the panel. Left most column will be for graphically displaying 
% the laser trace.
set(statusLvl_1H,'Spacing',space,'BackgroundColor','k');
set(statusLvl_0V,'Heights',[-30,-500]);

%%%%%%%%Create the area for the graph to live in.%%%%%%%%%%%%%%
%First Child of statusLvl1
statusGraph = uix.HBox('Parent',statusLvl_1H);
axes1 = axes( 'Parent', statusGraph, ...
    'ActivePositionProperty', 'outerposition' );


%%%%%%%%%%%%%%% Create Numerical Laser Status Region %%%%%%%%%%%%%%%%%%%%%%
%For laser status readings
numStatusHBox = uix.HBox('Parent',statusLvl_1H);

numStatusTxt = uix.VButtonBox('Parent',numStatusHBox,'VerticalAlignment','middle',...
    'HorizontalAlignment','right');
txtString = {'pwrOut_txt::Laser Power Out (W)','modCur_txt::Laser Module Current (A)',...
    'modTemp_txt::Laser Module Temp (C)'};
obj = obj.build_uicontrols(numStatusTxt,txtString,'Style','text','FontSize',20);
set(numStatusTxt,'spacing',25,'ButtonSize',[200,70]);

numStatusEdit = uix.VButtonBox('Parent',numStatusHBox,'VerticalAlignment','middle',...
    'HorizontalAlignment','left');
editString = {'pwrOut::NA','modCur::NA','modTemp::NA'};
obj = obj.build_uicontrols(numStatusEdit,editString,'Style','edit','FontSize',36,...
'Enable','inactive');
set(numStatusEdit,'spacing',8,'ButtonSize',[200,100]);
% set(statusNumerical,'Widths',[-2,-1],'Heights',[50 50,50],'spacing',25);


%%%%%% Create Laser Control Panel & Buttons %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

RightV =  uix.VBoxFlex('Parent',MainH);


LaserCtrl =  uix.Panel('Parent',RightV,...
    'BackgroundColor',colorMain, 'Title','Laser Control','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');%LaserCtrl is the 2nd Child of Main. Which 
%means it will be the second column in the 'Main' tab. It is also the right
%most column because Main has no other Children. 

ctrlButV = uix.VButtonBox('Parent',LaserCtrl,'VerticalAlignment','top');
ctrlString = {'reset_btn::Reset','key_btn::Laser Key','rStart_btn::Remote Start',...
    'prgmRun_rbtn::Programmed Run','manualRun_rbtn::Manual Run','emission_btn::Turn Emission On'};
set(ctrlButV,'ButtonSize',[160 25],'Spacing',5);
ctrlStyles = {'pushbutton','pushbutton','pushbutton','radiobutton','radiobutton',...
    'pushbutton'};
ctrlEnable = {'on','on','off','on','on','off'};

obj = obj.build_uicontrols(ctrlButV,ctrlString,'Value',0,'Style',ctrlStyles,...
    'Enable',ctrlEnable);


%Create the Laser Run Settings Panel in Left
laserRunPan = uix.Panel('Parent',LeftV,...
    'BackgroundColor',colorMain, 'Title','Laser Run Settings','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');%

laserRunCtrl = uix.HBox('Parent',laserRunPan);

progCtrlPan = uix.Panel('Parent',laserRunCtrl,'BackgroundColor',colorMain,...
    'Title','Programmed Control of Laser','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');

manualCtrlPan = uix.Panel('Parent',laserRunCtrl,'BackgroundColor',colorMain,...
    'Title','Manual Control of Laser','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');


%%%%%%%% Create the items inside of the Programmed Control section %%%%%%% 
progCtrl_lvl0 = uix.VBox('Parent',progCtrlPan);

waveformPans = uix.TabPanel('Parent',progCtrl_lvl0,'BackgroundColor',colorMain);
simple = uix.HBox('Parent',waveformPans);
simpInput_txt = uix.VButtonBox('Parent',simple);
simpInputString = {'min_txt::Min','max_txt::Man','delay_txt::Delay',...
    'onDur_txt::On Duration','offDur_txt::Off Duration','riseT_txt::Rise Time',...
    'fallT_txt::Fall Time'};

% set(ctrlButV,'ButtonSize',[160 25],'Spacing',5);
obj = obj.build_uicontrols(simpInput_txt,simpInputString,'Style','text');

simpInput = uix.VButtonBox('Parent',simple);
simpInputString = {'min::Min','max::Man','delay::Delay',...
    'onDur::On Duration','offDur::Off Duration','riseT::Rise Time',...
    'fall::Fall Time'};

% set(ctrlButV,'ButtonSize',[160 25],'Spacing',5);
obj = obj.build_uicontrols(simpInput,simpInputString,'Style','edit');


complex= uix.HBox('Parent',waveformPans);

   

%titling, setting the font, font size, widths, and colors for the tabs
waveformPans.TabTitles = {'Simple','Complex'};
waveformPans.FontSize = 12;
waveformPans.FontName = 'Times New Roman';
waveformPans.TabWidth = 130;


% waveFormSelection = uix.HButtonBox('Parent',progCtrl_lvl0,'VerticalAlignment','top',...
%     'HorizontalAlignment','left');
% dropDownStr = {'waveform_txt::Waveform Selection: ',{'waveform_popup::Simple','Complex'}};
% 
% obj = obj.build_uicontrols(waveFormSelection,dropDownStr,'Style',{'text','popup'});
% set(waveFormSelection,'spacing',2,'ButtonSize',[150,20]);



% Settings for each of the regions on the GUI: (has to come at the end
% because of the way GUI Layout Tool box handles sizes)

% Size control:

set(MainH,'Widths',[-(1500-200),-200]);% For main horizontal boxes (Status Area/Laser Run Settings & Laser Control
set(statusLvl_0V,'Heights',[-1,-8.5]);% For height sizing between laser status lights and graph & numerical status regions
set(statusLvl_1H,'Widths',[-3,-1]);% For width sizing between graph and laser numerical status regions





