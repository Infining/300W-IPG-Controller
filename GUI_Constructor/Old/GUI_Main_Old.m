
%% Create the overall window w/tabs
%Creating the window that everything resides in
f = figure('Position',[300,200,1500,700]);
f.ToolBar = 'none';
f.MenuBar = 'none';
f.NumberTitle = 'off';
f.Name = '300W IPG Laser Controller: V1.0';
obj = GUI_Ctrl();%create instance of class that builds buttons and handles 
%their functions

%Creating the parent for the tabs
panels = uix.TabPanel('Parent',f);

%Creating the 'Main' tab 
Main = uix.HBoxFlex('Parent',panels);
space = 3;
set(Main,'Spacing',space,'BackgroundColor','k');
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

Left = uix.VBoxFlex('Parent',Main);%Left is the 1st Child of Main. Which 
%means it will be the left most column in the 'Main' tab (and the only
%column unless we add more Children (which we will)). We made this a
%Vertical Box (or VBox) which lets us break Left into rows (depending on
%the number of Children Left has)
set(Left,'Spacing',space,'BackgroundColor','k');

%Create the Status Panel in 'Main' tab
StatusPan = uix.Panel('Parent',Left,...
    'BackgroundColor',colorMain, 'Title','Status Area','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');% 

topStatusColor = StatusPan.BackgroundColor;

statusLvl_0 = uix.VBox('Parent',StatusPan,'BackgroundColor',topStatusColor);
%statusLvl_0: 1st and only Child of StatusPan. Allows us to build rows 
%inside of the panel. Top most row will be for stauts lights
set(statusLvl_0,'Spacing',space-3,'BackgroundColor','k');
% Create the first row in statusLvl2_0 for the status lights (boxes that 
% change color when state changes). 1st Child of statusLvl_0
statusLights = uix.HButtonBox('Parent',statusLvl_0,...
    'HorizontalAlignment','center','VerticalAlignment','top');
set(statusLights,'ButtonSize',[160 30],'Spacing',5);
String = {'Laser Connected: No', 'Interlock #1: Open','Interlock #2: Open',...
    'Laser Ready: No','Guide Laser: Off'};

obj.build_StatusLights(statusLights,String);
% statusS.Connected = uicontrol('Parent',statusLights,'String','Laser Connected: No');
% statusS.Interlock1 = uicontrol('Parent',statusLights,'String','Interlock #1: Open');
% statusS.Interlock2 = uicontrol('Parent',statusLights,'String','Interlock #2: Open');
% statusS.laserReady = uicontrol('Parent',statusLights,'String','Laser Ready: No');
% statusS.guideLaser = uicontrol('Parent',statusLights,'String','Guide Laser: Off');
% names = fieldnames(statusS);
% %handle default settings for some of the status boxes
% for kk = 1:length(names)
%     statusS.(names{kk}).FontName = 'Times New Roman';
%     statusS.(names{kk}).FontSize = 12;
%     statusS.(names{kk}).ForegroundColor = 'k';
%     statusS.(names{kk}).BackgroundColor = 'r';
% end



statusLvl_1 = uix.HBoxFlex('Parent',statusLvl_0,'BackgroundColor',topStatusColor);
%statusLvl1: 2nd and last Child of statusLvl_0. Allows us to build columns 
%inside of the panel. Left most column will be for graphically displaying 
% the laser trace.
set(statusLvl_1,'Spacing',space,'BackgroundColor','k');
set(statusLvl_0,'Heights',[-30,-500]);

% Create the area for the graph to live in. First Child of statusLvl1
statusGraph = uix.HBox('Parent',statusLvl_1);
axes1 = axes( 'Parent', statusGraph, ...
    'ActivePositionProperty', 'outerposition' );

%Create the area the rest of the status related items will live in (to the
%right of the graph area). Second Child of statusLvl1
statusLvl2 = uix.VBoxFlex('Parent',statusLvl_1);
set(statusLvl2,'Spacing',space,'BackgroundColor','k');




%Create the second row in statusLvl2 for the remaining status indicators.
%2nd Child of statusLvl2
statusLvl3 = uix.HBox('Parent',statusLvl2);
set(statusLvl3,'Spacing',space,'BackgroundColor','k');

%Create the first column in statusLvl3. This area holds the numerical laser
%status outputs (i.e. Power, Current, and Module Temperature). 1st Child
%of statusLvl3
statusNumerical = uix.Grid('Parent',statusLvl3);

%Create the second column in statusLvl3. This will hold the laser is
%emmitting/not emitting indicator light

statusEmission = uicontrol('Parent',statusLvl3,'String','No Laser Emission');


% topStatus = uix.HButtonBox('Parent',StatusPan,'BackgroundColor',topStatusColor,...
%     'Position',[10,750,730,240]/1000,'HorizontalAlignment','left','VerticalAlignment','top');

% topStatus.Position = [10,750,730,240]/1000;

%






%%

LaserCtrl =  uix.Panel('Parent',Main,...
    'BackgroundColor',colorMain, 'Title','Laser Control','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');%LaserCtrl is the 2nd Child of Main. Which 
%means it will be the second column in the 'Main' tab. It is also the right
%most column because Main has no other Children. :(



% leftStatus = uix.Grid('Parent',StatusPan,'BackgroundColor','m');
% leftStatus.Position = [10,50,100,50]./1000;




% LaserSetup = uix.Panel('Parent',Left,...
%     'BackgroundColor','m', 'Title','Laser Setup','TitlePosition','centertop',...
%     'FontSize',12,'FontName','Times New Roman');

% LaserCtrl = uix.Panel('Parent',Main,...
%     'BackgroundColor','y', 'Title','Laser Control','TitlePosition','centertop',...
%     'FontSize',12,'FontName','Times New Roman');
CtrlBut = uix.VButtonBox('Parent',LaserCtrl);
uicontrol('Parent',CtrlBut,'String','Reset');
uicontrol('Parent',CtrlBut,'String','Laser Key');
uicontrol('Parent',CtrlBut,'String','Remote Start','Enable','off');
uicontrol('Parent',CtrlBut,'String','Enable Program','Enable','off');
uicontrol('Parent',CtrlBut,'String','Run Program');
uicontrol('Parent',CtrlBut,'String','Turn Laser On','Enable','off');


set(Main,'Widths',[-(1500-200),-200]);% For main horizontal boxes
% set(Left,'Spacing',10);%For main Vertical boxes (in left horizontal box)


%Create the Laser Run Panel in Left
laserRunPan = uix.Panel('Parent',Left,...
    'BackgroundColor',colorMain, 'Title','Laser Run Settings','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');%

laserRunCtrl = uix.HBox('Parent',laserRunPan);

progCtrl = uix.Panel('Parent',laserRunCtrl,'BackgroundColor',colorMain,...
    'Title','Programmed Control of Laser','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');

manualCtrl = uix.Panel('Parent',laserRunCtrl,'BackgroundColor',colorMain,...
    'Title','Manual Control of Laser','TitlePosition','centertop',...
    'FontSize',16,'FontName','Times New Roman');


% Settings for each of the regions on the GUI: (has to come at the end
% because of the way GUI Layout Tool box handles sizes)











