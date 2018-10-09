%disp(laserFun(0.0, 1.0, 2.0, 3.0, 4.0, 10.0, 100.0, -1.0, 0.02, 10.0));

fplot(@(t) laserFun(0.0, 1.0, 2.0, 3.0, 4.0, 10.0, 100.0, -10.0, t, 10.0), [0 10]);

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
                            tnew = t+R*Period; %adjust time to make below bounds still work                           
                        elseif R>0  %positive phase shift happened
                            tnew = t+(1-R)*Period;%adjust time to make below bounds still work                            
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