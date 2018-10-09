classdef GUI_Callback
    %Builds uicontrol objects for the 300W Controller GUI. Sets default values and assigns
    %specific values as defined in GUI_Builder.
    %
    %Stores all uicontrol handles in the handles property
    
    properties
       
        maxCurr = 100;
        minCurr = 10;
        maxPower = 300;
        minPower = 30;
%         serialObj
%         baudRate = 115200;
    end
    
    methods
        
        function CallObj = GUI_Callback
        end
        
        function CallObj = callBack(CallObj,handle,event,CtrlObj,BldObj)
            
            switch handle.Tag
                case 'reset_btn' % for the Reset Button
                   
                case 'key_btn' % for the 'Laser Key' button
%                 case 'rStart_btn'% for the 'Enable Remote Start' button
                case 'enabLaser_btn'
                    
                    
                case 'prgmRun_rbtn' % for the 'Programmed Run' radial button
                    handle.Value = 1;
                    CtrlObj.handles.manualRun_rbtn.Value = 0;
                    CallObj.panEnableCtrl('prgmRun_rbtn',CtrlObj);
                    CallObj.simplePrgmChange(CtrlObj);
                case 'manualRun_rbtn' %for the 'Manual Run' radial button
                    handle.Value = 1;
                    CtrlObj.handles.prgmRun_rbtn.Value = 0;
                    CallObj.panEnableCtrl('manualRun_rbtn',CtrlObj);
                    CallObj.manualChange(CtrlObj,'manCurrent')
                case 'emission_btn' % for the 'Turn Emission On' button
                    
% % %                     
                case {'min','max','delay','onDur','offDur','riseT','fallT','phase'} %for the Simple programmed area
                    CallObj.simplePrgmChange(CtrlObj);
                case {'endInput','endPopup'} %For the end condition area
                    if CtrlObj.panHandles.waveformPans.Selection == 1
                        %Simple panel tab is selected
                        CallObj.simplePrgmChange(CtrlObj)
                    elseif CtrlObj.panHandles.waveformPans.Selection == 2
                        %Complex panel tab is selected
                    end
                    switch CtrlObj.handles.endPopup.String{CtrlObj.handles.endPopup.Value}                        
                        case 'Cycles (#)'
                            CtrlObj.handles.endInput.Enable = 'on';
                        case 'Run Time (sec)'
                            CtrlObj.handles.endInput.Enable = 'on';
                        case 'User Stop'
                            CtrlObj.handles.endInput.Enable = 'off';
                    end
                case {'manPower','manCurrent'}                    
                    CallObj.manualChange(CtrlObj,handle.Tag)
                    
            end               
        end
        
        
        
        function simplePrgmChange(CallObj,CtrlObj)
            %handles what happens when user changes parameters in the Simple
            %tab
            graph = CtrlObj.handles.graph;
            [Min, Max,delay,onDur,offDur,riseT,fallT,phaseShift] = valueCheck(CtrlObj);
      
            Period = riseT+onDur+fallT+offDur;%period of signal
            userStop = false;
%             try
            switch CtrlObj.handles.endPopup.String{CtrlObj.handles.endPopup.Value}
                
                case 'Cycles (#)'
                    cycles = str2double(CtrlObj.handles.endInput.String);                 
                    endT = delay+Period*cycles;                    
                case 'Run Time (sec)'
                    laserTime = str2double(CtrlObj.handles.endInput.String);
                    endT = delay+laserTime;                    
                case 'User Stop'
                    userStop = true;
                    endT = delay+Period*2;%time we stop plotting but we will need to make the laser keep running
            end
%             catch
%                 1;
%             end
            time = 0:0.001:endT;
            laserVector = zeros(1,length(time));
            for kk = 1:length(time) 
                [laserVector(kk)] = laserFun(delay,riseT,onDur,fallT,offDur,Min,Max,phaseShift,time(kk),endT);
            end
            
            axes(graph);
            if userStop
               ind = (time > delay + Period);               
               plot(time(~ind),laserVector(~ind),time(ind(1:end-1)),laserVector(ind(1:end-1)),'--r')                
            else
                plot(time, laserVector)
            end
            ylim(graph,[-5,105])
            xlim(graph,[-5,max(time)+5])
            xlabel('Time (sec)')
            ylabel('Current (%)')
            title('Expected Laser Behavior & Live Laser Behavior')
            
            function [Value] = laserFun(delay,riseT,onDur,fallT,offDur,Min,Max,phaseShift,t,endT)
                %function that will also be in control board to determine
                %current to the laser. Board version also require End Condition
                %(Specifically User End) so it knows how to handle that
                
                if t>= endT %stop laser
                    Value = 0;
                else % determine what value we have
                    Period = riseT+onDur+fallT+offDur;%period of signal
                    R = mod(phaseShift/Period,1).*phaseShift/abs(phaseShift);
                    if isnan(R)
                        R = 0;
                    end
                    
                    if t<=delay %laser hasn't turned on yet
                        Value = 0;
                    else
                        if R<0  %negative phase shift happened
                            tnew = t-(1+R)*Period; %adjust time to make below bounds still work                           
                        elseif R>0  %positive phase shift happened
                            tnew = t+(1-R)*Period;%adjust time to make below bounds still work  
%                             tnew = t+(R)*Period;%adjust time to make below bounds still work 
                        elseif R == 0
                            tnew = t;
                        end
                        n = ceil((tnew-delay)/Period);
                        riseStart = delay+(n-1)*Period; riseStop =delay+riseT+(n-1)*Period;
                        maxStart = delay+riseT+(n-1)*Period; maxStop = delay+riseT+onDur+(n-1)*Period;
                        fallStart = delay+riseT+onDur+(n-1)*Period; fallStop = delay+riseT+onDur+fallT+(n-1)*Period;
                        minStart = delay+riseT+onDur+fallT+(n-1)*Period; minStop = delay+riseT+onDur+fallT+offDur+(n-1)*Period;
                                                
                        if tnew>= riseStart && tnew <= riseStop %Laser is on a rise ramp
                            Value = (Max-Min)/riseT*(tnew-delay-(n-1)*Period)+Min;
                        elseif tnew >= maxStart && tnew<= maxStop %laser is on the max on region
                            Value = Max;
                        elseif tnew >= fallStart && tnew <= fallStop %laser is on the fall down ramp
                            Value = (Min-Max)/fallT*(tnew-delay-riseT-onDur-(n-1)*Period)+Max;
                        elseif tnew >= minStart && tnew <= minStop  %laser is in the off duration (or low state)
                            Value = Min;
                        else %to catch rare case of machine error and multiple bounds having the same start and stop values
                            [~,inds] = min(abs(tnew-[riseStart,riseStop,maxStart,maxStop,fallStart,fallStop,minStart,minStop]));
                            switch inds(1)
                                case {1,2}
                                    Value = (Max-Min)/riseT*(tnew-delay-(n-1)*Period)+Min;
                                case {3,4}
                                     Value = Max;
                                case {5,6}
                                    Value = (Min-Max)/fallT*(tnew-delay-riseT-onDur-(n-1)*Period)+Max;
                                case {7,8}
                                     Value = Min;
                            end                            
                        end                        
                    end
                end
            end
            
            function [Min, Max,delay,onDur,offDur,riseT,fallT,phaseShift] = valueCheck(CtrlObj)
                Min = str2double(CtrlObj.handles.min.String);
                Max = str2double(CtrlObj.handles.max.String);
                delay = str2double(CtrlObj.handles.delay.String);
                onDur = str2double(CtrlObj.handles.onDur.String);
                offDur = str2double(CtrlObj.handles.offDur.String);
                riseT = str2double(CtrlObj.handles.riseT.String);
                fallT = str2double(CtrlObj.handles.fallT.String);
                phaseShift = str2double(CtrlObj.handles.phase.String);
                if Max>100
                    Max = 100; CtrlObj.handles.max.String = '100';
                end
                if Min<0
                    Min = 0; CtrlObj.handles.min.String = '0';
                end
                if delay < 0
                    delay = 0; CtrlObj.handles.delay.String = '0';
                end
                if onDur < 0
                    onDur = 0; CtrlObj.handles.onDur.String = '0';
                end
                if offDur < 0
                    offDur = 0; CtrlObj.handles.offDur.String = '0';
                end
                if riseT < 0
                    riseT = 0; CtrlObj.handles.riseT.String = '0';
                end
                if fallT < 0
                    fallT = 0; CtrlObj.handles.fallT.String = '0';
                end
            end
        end
        
        function panEnableCtrl(CallObj,rbtn,CtrlObj)
%             Allows panels to be grayed out when disabling items inside of
%             them
            
            switch rbtn
                case 'prgmRun_rbtn'
                    prgmEnable = 'on';
                    manEnable = 'off';
                case 'manualRun_rbtn'
                    %User wants to manually control laser
                    %disable all of the controls in Programmed Control of
                    %Laser
                    prgmEnable = 'off';
                    manEnable = 'on';
            end
            recursiveEnableCtrl(CtrlObj.panHandles.prgmCtrlPan.Children,prgmEnable);
            recursiveEnableCtrl(CtrlObj.panHandles.manualCtrlPan.Children,manEnable);
            
            function recursiveEnableCtrl(Child,enableState)
                if isscalar(Child)
                    switch Child.Type
                        case 'uicontrol'
                            Child.Enable = enableState;
                        case 'uicontainer'
                            if ~isempty(Child.Children)
                                recursiveEnableCtrl(Child.Children,enableState)
                            end
                    end
                else
                    for kk = 1:length(Child)
                        recursiveEnableCtrl(Child(kk),enableState)
                    end
                end
            end
        end
        
        function manualChange(CallObj,CtrlObj,fieldChanged)
            %handles what happens when user changes parameters in the
            %Manual Control of the Laser area
            Value = str2double(CtrlObj.handles.(fieldChanged).String);
            graph = CtrlObj.handles.graph;    
            axes(graph);   
            switch fieldChanged
                case 'manPower'
                    if Value > CallObj.maxPower 
                        Value = CallObj.maxPower;
                    elseif Value < CallObj.minPower && Value ~= 0
                        Value = CallObj.minPower;
                    end
                    CtrlObj.handles.manCurrent.String = num2str(Value/CallObj.maxPower*CallObj.maxCurr);
                    ylabel('Power (W)')
                    yLim =[-5,305];
                case 'manCurrent'
                    if Value > CallObj.maxCurr
                        Value = CallObj.maxCurr;
                    elseif Value < CallObj.minCurr && Value ~= 0
                        Value = CallObj.minCurr;
                    end
                    CtrlObj.handles.manPower.String = num2str(Value/CallObj.maxCurr*CallObj.maxPower);
                    ylabel('Current (%)')
                    yLim =[-5,105];
            end
            CtrlObj.handles.(fieldChanged).String = num2str(Value);
            time = 0:.01:30;
            laserVector = ones(1,length(time)).*Value;
            laserVector(1) = 0;
%             inds = (time>max(time)/2);
            plot(time(1:2),laserVector(1:2),time(3:end),laserVector(3:end),'--r')
            ylim(yLim)
            xlim([-5,max(time)+5])
            
        end
        
    end
end

