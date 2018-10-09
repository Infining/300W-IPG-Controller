classdef GUI_Builder
    %Builds the GUI structure & calls the functions which will populate it
    %with uicontrol objects.
    
    properties
        %%Color settings Main Tab:%%
        colorMain = [204,255,255]./255;%Color used for the main panel headers 
        subPanColor = [240,255,240]/255;%Color used for sub panel headers (i.e. Programmed Control of Laser and Manual Control of Laser
        MainHLineColor = 'k';%Color of primary horizontal line that divides Main in two
        StatCtrlVLine = 'k';%Color of vertical line/ border between Laser Control & Status Area
        RunNotificationVLine = 'k'; %Color of vertical line between Laser Run Settings & Notification Area
        %Colors for Laser Run Settings Area        
        simpleTabColor = ([100,116,132]+100)/255;%Color used for Simple, Complex & Manual control of laser backgrounds
        endConditionColor = [240,248,255]/255;%Color used for End Condition background region
        prgmControlBkgrd = [192,192,192]/255; %Color used for area behind the Simple and Complex tabs
        
        %Colors for Status Area
        StatHLine = 'k'; %color of top horizontal line in status area
        StatVLine = 'k';%color of vertical line in status area
        graphRegion = [255 250 240]/255;%color of region around graph
        statLightArea = [220 220 220]/255;
        %Colors for Laser Control Area
        LCbkgrd = [255 245 238]/255;
        %Colors for Notification Area 
        
        %%Colors Errors & Status Tab%%
        ErrorTabBckgrd = [229,255,204]./255;
%         Errorpan
        font = 'Times';
        space = 3;
    end
    methods
        function BldObj = GUI_Builder(varargin)
            [BldObj] = globalSettings(BldObj, varargin);
            
            
        end
        
        %Creates main figure & makes it divisible into tab panels
        function [f,CtrlObj] = mainFig(BldObj,CtrlObj,SerialObj)
            f = figure('Position',[300,200,1500,700]);
            f.ToolBar = 'figure';
            f.MenuBar = 'none'; 
            tbh = findall(f,'Type','uitoolbar');
            allHands = findall(tbh);
%             toolbarTags = {'FigureToolBar','Plottools.PlottoolsOn',...
%                 'Plottools.PlottoolsOff','Annotation.InsertLegend',...
%                 'Annotation.InsertColorbar','DataManager.Linking',...
%                 'Exploration.Brushing','Exploration.DataCursor',...
%                 'Exploration.Rotate','Exploration.Pan','Exploration.ZoomOut',...
%                 'Exploration.ZoomIn','Standard.EditPlot','Standard.PrintFigure',...
%                 'Standard.SaveFigure','Standard.FileOpen','Standard.NewFigure'};
            tags2Delete = [2,3,4,5,6,14,15,16,17];
            delete(allHands(tags2Delete));
            f.NumberTitle = 'off';
            f.Name = '300W IPG Laser Controller: V1.0';
            CtrlObj.panHandles.masterPanels = uix.TabPanel('Parent',f);
            
            %Create menu bars
            
            edit = uimenu(f,'Label','Edit','Callback',@SerialObj.availablePorts); 
            con = uimenu(edit,'Label','Connect'); 
            uimenu(con,'Label','Serial');            
            uimenu(con,'Label','Ethernet');
            
        end
        
        %Create 1st tab inside of Main Figure & and makes it divisible into columns: Main
        function [BldObj,CtrlObj] = Tabs(BldObj,CtrlObj,masterPanels)
            %Creating the 'Main' tab
            Main = uix.VBoxFlex('Parent',masterPanels);
            top_Main = uix.HBoxFlex('Parent',Main);
%             right_Main =  uix.VBoxFlex('Parent',Main);
            bot_Main = uix.HBoxFlex('Parent',Main);
            %Creating the 'Error & Warnings' tab
            Error = uix.HBox('Parent',masterPanels);
            
            
            %Settings for tabs
            set(Main,'Spacing',BldObj.space,'BackgroundColor',BldObj.MainHLineColor);
%             set(Main,'Widths',[-(1500-200),-200]);% For main horizontal boxes (Status Area/Laser Run Settings & Laser Control)
            set(top_Main,'Spacing',BldObj.space,'BackgroundColor',BldObj.StatCtrlVLine);
            set(bot_Main,'Spacing',BldObj.space,'BackgroundColor',BldObj.RunNotificationVLine);
            masterPanels.TabTitles = {'Main','Errors & Status'};
            masterPanels.FontSize = 12;
            masterPanels.FontName = BldObj.font;
            masterPanels.TabWidth = 130;
            Error.BackgroundColor = BldObj.ErrorTabBckgrd;
            
            [CtrlObj,BldObj] = BldObj.statusArea(CtrlObj,top_Main);
            [CtrlObj,BldObj] = BldObj.laserCtrlArea(CtrlObj,top_Main);
            %Settings for top_Main:
            set(top_Main,'Widths',[-(1500-200),-200]);% For top horizontal boxes (Status Area/Laser Run Settings & Laser Control)
            
            
            [CtrlObj,BldObj] = BldObj.laserRunSettings(CtrlObj,bot_Main);
            [CtrlObj,BldObj] = BldObj.notificationArea(CtrlObj,bot_Main);
            
            [CtrlObj,BldObj] = BldObj.Error_StatArea(CtrlObj,Error);
            set(Error,'Widths',[-1,-1,-3]);
        end
        
        function [CtrlObj,BldObj] = Error_StatArea(BldObj,CtrlObj,Error) 
            %Create Error Panel
            CtrlObj.panHandles.errorPan = uix.Panel('Parent',Error,...
                'BackgroundColor',BldObj.colorMain, 'Title','Errors','TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);%
           
            errorPan = CtrlObj.panHandles.errorPan;
            errorPan_Lvl0 = uix.HBox('Parent', errorPan);
            LEDwidth = 25;
            LEDheight = 25;
            %Create LEDs for errors
            errorLEDs = uix.VButtonBox('Parent', errorPan_Lvl0,...
                    'HorizontalAlignment','left','VerticalAlignment','top',...
                    'BackgroundColor','w');
               
            set(errorLEDs,'ButtonSize',[LEDwidth LEDheight],'Spacing',25);
            String = {'highTemp_LED','lowTemp_LED','backReflect_LED','modCom_LED',...
                'modHealth_LED','expectedEmiss_LED','modPSerr_LED','leakCurr_LED'};
%             Color = {'r','k','m','g','w','y','b','c'};
            Color = 'y';
            CtrlObj = CtrlObj.build_uicontrols(errorLEDs,String,'Style','LED','Color',Color);                 
            
            %Create Text for Error LEDs
            errorTxt = uix.VButtonBox('Parent', errorPan_Lvl0,...
                    'HorizontalAlignment','left','VerticalAlignment','top',...
                    'BackgroundColor','w');
                
            set(errorTxt,'ButtonSize',[200 25],'Spacing',25);
            StringTxt = {'highTemp::High Temperature: N/A','lowTemp::Low Temperature: N/A',...
                'backReflect::Back Reflection: N/A','modCom::Module Disconnnected: N/A',...
                'modHealth::Module: N/A','expectedEmiss::Emission Control: N/A',...
                'modPSerr::Module Power Supply: N/A','leakCurr::Leakage Current: N/A'};

            CtrlObj = CtrlObj.build_uicontrols(errorTxt,StringTxt,'Style','text','HorizontalAlignment','left');
            set(errorPan_Lvl0,'Widths',[-.15,-1]);
            
            %Create Status Panel            
            CtrlObj.panHandles.statusPan = uix.Panel('Parent',Error,...
                'BackgroundColor',BldObj.colorMain, 'Title','Status','TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);%
           
            statusPan = CtrlObj.panHandles.statusPan;
            statusPan_Lvl0 = uix.HBox('Parent', statusPan);
            
            %Create LEDs for errors
            statusLEDs = uix.VButtonBox('Parent', statusPan_Lvl0,...
                    'HorizontalAlignment','left','VerticalAlignment','top',...
                    'BackgroundColor','w');
               
            set(statusLEDs,'ButtonSize',[LEDwidth LEDheight],'Spacing',25);
            String = {'EmissionOn_LED','GuideBeam_LED','PowerSupOn_LED','LaserEnabled_LED',...
                'EmissionSafeStart_LED','KeySwitchPos_LED','GuideHrdwreCtrl_LED','ModEnabled_LED','Interlock_LED'};
%             Color = {'r','k','m','g','w','y','b','c'};
            Color = 'y';
            CtrlObj = CtrlObj.build_uicontrols(statusLEDs,String,'Style','LED','Color',Color);            
            
            %Create Text for Error LEDs
            statusTxt = uix.VButtonBox('Parent', statusPan_Lvl0,...
                    'HorizontalAlignment','left','VerticalAlignment','top',...
                    'BackgroundColor','w');
                
            set(statusTxt,'ButtonSize',[200 25],'Spacing',25);
            StringTxt = {'EmissionOn::Emission: N/A','GuideBeam::Guide Beam: N/A ',...
                'PowerSupOn::Power Supply: N/A','LaserEnabled::Laser Enabled: N/A',...
                'EmissionSafeStart::Laser in Safe Start: N/A','KeySwitchPos::Key Set To: N/A ',...
                'GuideHrdwreCtrl::Ctrl of Guide Laser: N/A','ModEnabled::Modules Enabled: N/A','Interlock::Interlock: N/A'};

            CtrlObj = CtrlObj.build_uicontrols(statusTxt,StringTxt,'Style','text','HorizontalAlignment','left');
             set(statusPan_Lvl0,'Widths',[-.15,-1]);
            uix.Empty('Parent',Error);
            
             
            
        end
        
        function [CtrlObj,BldObj] = statusArea(BldObj,CtrlObj,top_Main) %#ok<*DEFNU>
            
            statusPan = uix.Panel('Parent',top_Main,...
                'BackgroundColor',BldObj.colorMain, 'Title','Status Area','TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);%
             
%             topStatusColor = statusPan.BackgroundColor;
            
            statusLvl_0V = uix.VBox('Parent',statusPan);
            %statusLvl_0: 1st and only Child of StatusPan. Allows us to build rows
            %inside of the panel. Top most row will be for stauts lights
            set(statusLvl_0V,'Spacing',BldObj.space-3,'BackgroundColor',BldObj.StatHLine);
            
            %Build the status lights
            [~,CtrlObj] = statusLights(BldObj,CtrlObj,statusLvl_0V);
            
            %Create region in status panel for graphical & numerical status of the laser
            statusLvl_1H = uix.HBoxFlex('Parent',statusLvl_0V);
            %statusLvl1: 2nd and last Child of statusLvl_0. Allows us to build columns
            %inside of the panel. Left most column will be for graphically displaying
            % the laser trace.
            set(statusLvl_1H,'Spacing',BldObj.space,'BackgroundColor',BldObj.StatVLine);
            set(statusLvl_0V,'Heights',[-30,-500]);
            
            %Create the area for the graph to live in. 1st Child of statusLvl1
            statusGraph = uix.HBox('Parent',statusLvl_1H,'BackgroundColor',BldObj.graphRegion);
            
            CtrlObj.handles.graph = axes( 'Parent', statusGraph, ...
                'ActivePositionProperty', 'outerposition' );
            CtrlObj.handles.graph.Units = 'normalized';
%             showplottool(CtrlObj.handles.graph,'figurepalette')
            
            %Create the area for the numerical statuses of the laser are displayed
            %2nd Child of statusLvl1
            [~,~,~,CtrlObj] =...
                numericalLaserStatus(CtrlObj,statusLvl_1H);
            CtrlObj.panHandles.statusPan = statusPan;
            
            %Settings for statusArea
            set(statusLvl_0V,'Heights',[-1,-8.5]);% For height sizing between laser status lights and graph & numerical status regions
            set(statusLvl_1H,'Widths',[-3,-1]);% For width sizing between graph and laser numerical status regions
                       
            function [statusLightsH,CtrlObj]= statusLights(BldObj,CtrlObj,statusLvl_0V)
            %Builds the status lights
                statusLightsH = uix.HButtonBox('Parent',statusLvl_0V,...
                    'HorizontalAlignment','center','VerticalAlignment','middle',...
                    'BackgroundColor',BldObj.statLightArea);
                set(statusLightsH,'ButtonSize',[160 25],'Spacing',5);
                String = {'connected_galileo::Galileo Connected: No','connected_SL::Laser Connected: No', 'lock1_SL::Interlock #1: Open',...
                    'lock2_SL::Interlock #2: Open','ready_SL::Laser Ready: No',...
                    'guideLaser_SL::Guide Laser: Off','emission_SL::Laser Emission: Off'};
                CtrlObj = CtrlObj.build_uicontrols(statusLightsH,String,'BackgroundColor',{'r','r','r','r','r','r','r'},'Enable','inactive');
                
            end
                        
            function [numStatusHBox,numStatusTxt,numStatusEdit,CtrlObj] = numericalLaserStatus(CtrlObj,statusLvl_1H)
            %Create Numerical Laser Status Region    
                %For laser status readings
                numStatusHBox = uix.HBox('Parent',statusLvl_1H);                
                numStatusTxt = uix.VButtonBox('Parent',numStatusHBox,'VerticalAlignment','middle',...
                    'HorizontalAlignment','right');
                txtString = {'pwrOut_txt::Laser Power Out (W)','modCur_txt::Laser Module Current (A)',...
                    'modTemp_txt::Laser Module Temp (\circC)'};
                CtrlObj = CtrlObj.build_uicontrols(numStatusTxt,txtString,'Style','text','FontSize',20);
                set(numStatusTxt,'spacing',8,'ButtonSize',[200,70]);
                
                numStatusEdit = uix.VButtonBox('Parent',numStatusHBox,'VerticalAlignment','middle',...
                    'HorizontalAlignment','left');
                editString = {'pwrOut::NA','modCur::NA','modTemp::NA'};
                CtrlObj = CtrlObj.build_uicontrols(numStatusEdit,editString,'Style','edit','FontSize',36,...
                    'Enable','inactive');
                set(numStatusEdit,'spacing',8,'ButtonSize',[170,70]);
            end            
        end
              
        function [CtrlObj,BldObj,ctrlButV] = laserCtrlArea(BldObj,CtrlObj,top_Main)
            CtrlObj.panHandles.LaserCtrl =  uix.Panel('Parent',top_Main,...
                'BackgroundColor',BldObj.colorMain, 'Title','Laser Control','TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);%LaserCtrl is the 2nd Child of Main. Which
            %means it will be the second column in the 'Main' tab. It is also the right
            %most column because Main has no other Children.
            LaserCtrl = CtrlObj.panHandles.LaserCtrl;
            ctrlButV = uix.VButtonBox('Parent',LaserCtrl,'BackgroundColor',BldObj.LCbkgrd);
            ctrlString = {'reset_btn::Reset','key_btn::Power Key','enabLaser_btn::Enable Laser',...
                'prgmRun_rbtn::Programmed Run','manualRun_rbtn::Manual Run','emission_btn::Turn Emission On'};
            
            ctrlStyles = {'pushbutton','pushbutton','pushbutton','radiobutton','radiobutton',...
                'pushbutton'};
            ctrlEnable = {'off','off','off','on','on','off'};
            Tips = {'Resets any resttable errors', 'Resets laser after interlock is broken',...
                'Enables laser emission','Run laser using program','Laser controlled manually by user',...
                'Turns on emission'};
            CtrlObj = CtrlObj.build_uicontrols(ctrlButV,ctrlString,'Value',0,'Style',ctrlStyles,...
                'Enable',ctrlEnable,'BackgroundColor',{[255 127 0]/255},'FontSize',13,...
                'TooltipString',Tips);
            
            CtrlObj.handles.prgmRun_rbtn.Value = 1;
            %Settings for Laser Control Area (for width of that region see Tabs
            %function for settings)
            set(ctrlButV,'ButtonSize',[160 40],'Spacing',5,'VerticalAlignment','middle');
            CtrlObj.handles.prgmRun_rbtn.BackgroundColor = BldObj.LCbkgrd;
            CtrlObj.handles.manualRun_rbtn.BackgroundColor = BldObj.LCbkgrd;
        end
        
        function [CtrlObj,BldObj] = notificationArea(BldObj,CtrlObj,bot_Main)
            CtrlObj.panHandles.notiAreaPan = uix.Panel('Parent',bot_Main,...
                'Title','Notification Area');%
            notiAreaPan = CtrlObj.panHandles.notiAreaPan;
            notiVBox = uix.VBox('Parent',notiAreaPan);
            CtrlObj.panHandles.notiScrollPan = uix.ScrollingPanel('Parent',...
                notiVBox);
            notiScrollPan = CtrlObj.panHandles.notiScrollPan;
            
             CtrlObj = CtrlObj.build_uicontrols(notiScrollPan,'NotiAreaTxt::Hello World',...
                 'Style','text','Enable','inactive','FontSize',12,...
                 'VerticalAlignment','top','HorizontalAlignment','left',...
                 'LineStyle','-','EdgeColor','k','BackgroundColor','w',...
                 'Margin',5);
%              set(notiScrollPan, 'Widths', -600, 'Heights', 500, 'HorizontalOffsets', 0, 'VerticalOffsets', 0 )
%             set(notiScrollPan, 'Widths', -600)
             
            empt = uix.Empty('Parent',notiVBox);
             
            set(notiVBox,'Heights',[-1,-.005]);
            
            set(notiAreaPan,'BackgroundColor',BldObj.colorMain,'TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);
        end
        
        %Builds the Laser Run Settings area in the GUI
        function [CtrlObj,BldObj] = laserRunSettings(BldObj,CtrlObj,bot_Main)
            CtrlObj.panHandles.laserRunPan = uix.Panel('Parent',bot_Main,...
                'Title','Laser Run Settings');%
            laserRunPan = CtrlObj.panHandles.laserRunPan;
            
            laserRunCtrl = uix.HBoxFlex('Parent',laserRunPan,'Spacing',BldObj.space);
            
            CtrlObj.panHandles.prgmCtrlPan = uix.Panel('Parent',laserRunCtrl,...
                'Title','Programmed Control of Laser');
            prgmCtrlPan = CtrlObj.panHandles.prgmCtrlPan;
            
            CtrlObj.panHandles.manualCtrlPan = uix.Panel('Parent',laserRunCtrl,...
                'Title','Manual Control of Laser');
            manualCtrlPan = CtrlObj.panHandles.manualCtrlPan;
            
            [CtrlObj,BldObj] = BldObj.programmedCtrlArea(CtrlObj,prgmCtrlPan);
            [CtrlObj,BldObj] = BldObj.manualCtrlArea(CtrlObj,manualCtrlPan);
            
            %Settings:
            
            %For the overall panel holding both the Programmed and Manual Panles
            set(laserRunPan,'BackgroundColor',BldObj.colorMain,'TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);
            
            %Settings for the Programmed Control of Laser Panel             
            set(prgmCtrlPan,'BackgroundColor',BldObj.subPanColor,'TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);            
            set(laserRunCtrl,'Widths',[-2,-1]);% For main horizontal boxes (Status Area/Laser Run Settings & Laser Control)
            %settings for manual control
            set(manualCtrlPan,'BackgroundColor',BldObj.subPanColor,'TitlePosition','centertop',...
                'FontSize',16,'FontName',BldObj.font);
            set(laserRunCtrl,'BackgroundColor','k')
            
            
        end
        
        function [CtrlObj,BldObj] = programmedCtrlArea(BldObj,CtrlObj,progCtrlPan)            
            
            progCtrl_lvl0 = uix.VBox('Parent',progCtrlPan);            
            CtrlObj.panHandles.waveformPans = uix.TabPanel('Parent',progCtrl_lvl0);
            waveformPans = CtrlObj.panHandles.waveformPans;
            %To control the spacing between the editable boxes and the top
            %of the Simple tab
            %put empty box here
            
            %Create Simple Controls Tab
            simple = uix.HBox('Parent',waveformPans);
            
            %Left most column of text:
            simpInput_txt1 = uix.VButtonBox('Parent',simple);
            simpInputStringtxt1 = {'min_txt::Min (% Current)','max_txt::Max (% Current)','delay_txt::Delay (sec)',...
                'onDur_txt::On Duration (sec)'};  
            CtrlObj = CtrlObj.build_uicontrols(simpInput_txt1,simpInputStringtxt1,'Style','text',...
                'HorizontalAlignment','left','BackgroundColor',{BldObj.simpleTabColor});
            
            %Left Column of input boxes:
            simpInput1 = uix.VButtonBox('Parent',simple);
            simpInputString1 = {'min::0','max::100','delay::15','onDur::10'};
            CtrlObj = CtrlObj.build_uicontrols(simpInput1,simpInputString1,'Style','edit');
            
            %Right Column of text:
            simpInput_txt2 = uix.VButtonBox('Parent',simple);
            simpInputStringtxt2 = {'offDur_txt::Off Duration (sec)','riseT_txt::Rise Time (sec)',...
                'fallT_txt::Fall Time (sec)','phase_txt::Phase Shift (sec)'};  
            CtrlObj = CtrlObj.build_uicontrols(simpInput_txt2,simpInputStringtxt2,'Style','text',...
                'HorizontalAlignment','left','BackgroundColor',{BldObj.simpleTabColor});
            
            %Right Column of input boxes:
            simpInput2 = uix.VButtonBox('Parent',simple);
            simpInputString2 = {'offDur::5','riseT::0',...
                'fallT::3','phase::0'};
            CtrlObj = CtrlObj.build_uicontrols(simpInput2,simpInputString2,'Style','edit');
            
            %Create Complex Controls Tab
            complex= uix.HBox('Parent',waveformPans);            
            
            %Create End Condition Area
            endCondition = uix.HButtonBox('Parent',progCtrl_lvl0);
            endCondString = {'end_txt::End Condition: ','endInput::1',{'endPopup::Cycles (#)','Run Time (sec)','User Stop'}};
            CtrlObj = CtrlObj.build_uicontrols(endCondition,endCondString,'Style',{'text','edit','popup'});
            set(endCondition,'ButtonSize',[115 25],'HorizontalAlignment','left',...
                'spacing',5,'VerticalAlignment','middle');%[w,h]
            
            %Settings for Programmed Control of Laser area:
            
            set(progCtrl_lvl0,'BackgroundColor',BldObj.prgmControlBkgrd ) %for container
%             behind Simple and Complex tab           
            waveformPans.TabTitles = {'Simple','Complex'};
            set(waveformPans,'FontSize',12,'FontName',BldObj.font,'TabWidth',130);
            set(waveformPans,'BackgroundColor',BldObj.simpleTabColor)
            
            set(simple,'Widths',[-1,-1,-1,-1]); %sets widths of text & edit columns Simple tab             

            %Colors of the Simple tab components
            set(simpInput1,'BackgroundColor',BldObj.simpleTabColor)
            set(simpInput2,'BackgroundColor',BldObj.simpleTabColor)
            set(simpInput_txt1,'BackgroundColor',BldObj.simpleTabColor)
            set(simpInput_txt2,'BackgroundColor',BldObj.simpleTabColor)
            set(simple,'BackgroundColor',BldObj.simpleTabColor)%set the color of the lines dividing the regions in Simple tab
            
            set(endCondition,'BackgroundColor',BldObj.endConditionColor)
            set(CtrlObj.handles.end_txt,'BackgroundColor',BldObj.endConditionColor)
            
            set(simpInput_txt1,'ButtonSize',[360 25],'Spacing',2,...
                'HorizontalAlignment','right','VerticalAlignment','top');
            set(simpInput1,'ButtonSize',[100 25],'Spacing',2,...
                'HorizontalAlignment','left','VerticalAlignment','top');
            set(simpInput_txt2,'ButtonSize',[360 25],'Spacing',2,...
                'HorizontalAlignment','right','VerticalAlignment','top');
            set(simpInput2,'ButtonSize',[100 25],'Spacing',2,...
                'HorizontalAlignment','left','VerticalAlignment','top');
           
            %Set sizes of veritcal boxes (tabs, end conditions, & descriptions)
            set(progCtrl_lvl0,'Heights',[-4,-1],'spacing',BldObj.space); 
            
        end
        
        function [CtrlObj,BldObj] = manualCtrlArea(BldObj,CtrlObj,manualCtrlPan)
            manCtrl_lvl0 = uix.HBox('Parent',manualCtrlPan);
            manTxtBox = uix.VButtonBox('Parent',manCtrl_lvl0);
            manEditBox = uix.VButtonBox('Parent',manCtrl_lvl0);
            
            manTxtString = {'manSet_txt::Set:','manPower_txt::Power (W)','manOr_txt::or','manCurrent_txt::Current (%)'};
            CtrlObj = CtrlObj.build_uicontrols(manTxtBox,manTxtString,'Style','text','Enable','off',...
                'FontSize',16,'BackgroundColor',{BldObj.simpleTabColor});
            
            manString = {'empty1_txt:: ','manPower::0','empty2_txt:: ','manCurrent::0'};
            CtrlObj = CtrlObj.build_uicontrols(manEditBox,manString,'Style',...
                {'text','edit','text','edit'},'Enable','off','FontSize',16);            
            
            set(manTxtBox,'ButtonSize',[125 40],'HorizontalAlignment','right',...
                'spacing',0,'VerticalAlignment','top');%[w,h]           
            set(manEditBox,'ButtonSize',[85 40],'HorizontalAlignment','left',...
                'spacing',0,'VerticalAlignment','top');%[w,h]   
            
            CtrlObj.handles.empty1_txt.BackgroundColor = BldObj.simpleTabColor;
            CtrlObj.handles.empty2_txt.BackgroundColor = BldObj.simpleTabColor;
            set(manTxtBox,'BackgroundColor',BldObj.simpleTabColor)
            set(manEditBox,'BackgroundColor',BldObj.simpleTabColor)
        end
        
    end    
end

