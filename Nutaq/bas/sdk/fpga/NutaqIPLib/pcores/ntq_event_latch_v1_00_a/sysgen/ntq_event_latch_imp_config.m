
function ntq_event_latch_imp_config(this_block)

  this_block.setTopLevelLanguage('VHDL');

  this_block.setEntityName('ntq_event_latch_imp');

  % System Generator has to assume that your entity  has a combinational feed through; 
  %   if it  doesn't, then comment out the following line:
  this_block.tagAsCombinational;

  this_block.addSimulinkInport('i_Rst_p');
  this_block.addSimulinkInport('i_ClkEn_p');
  this_block.addSimulinkInport('i_ArmLatch_p');
  this_block.addSimulinkInport('i_Trigger_p');
  this_block.addSimulinkInport('iv_Data_p');
  this_block.addSimulinkInport('i_DataWen_p');

  this_block.addSimulinkOutport('ov_LatchData_p');
  this_block.addSimulinkOutport('o_TriggerHappened_p');

  ov_LatchData_p_port = this_block.port('ov_LatchData_p');
  ov_LatchData_p_port.setType('UFix_64_0');
  o_TriggerHappened_p_port = this_block.port('o_TriggerHappened_p');
  o_TriggerHappened_p_port.setType('UFix_1_0');
  o_TriggerHappened_p_port.useHDLVector(false);

  % -----------------------------
  if (this_block.inputTypesKnown)
    % do input type checking, dynamic output type and generic setup in this code block.

    if (this_block.port('i_Rst_p').width ~= 1);
      this_block.setError('Input data type for port "i_Rst_p" must have width=1.');
    end

    this_block.port('i_Rst_p').useHDLVector(false);

    if (this_block.port('i_ClkEn_p').width ~= 1);
      this_block.setError('Input data type for port "i_ClkEn_p" must have width=1.');
    end

    this_block.port('i_ClkEn_p').useHDLVector(false);

    if (this_block.port('i_ArmLatch_p').width ~= 1);
      this_block.setError('Input data type for port "i_ArmLatch_p" must have width=1.');
    end

    this_block.port('i_ArmLatch_p').useHDLVector(false);

    if (this_block.port('i_Trigger_p').width ~= 1);
      this_block.setError('Input data type for port "i_Trigger_p" must have width=1.');
    end

    this_block.port('i_Trigger_p').useHDLVector(false);

    if (this_block.port('iv_Data_p').width ~= 64);
      this_block.setError('Input data type for port "iv_Data_p" must have width=64.');
    end

    if (this_block.port('i_DataWen_p').width ~= 1);
      this_block.setError('Input data type for port "i_DataWen_p" must have width=1.');
    end

    this_block.port('i_DataWen_p').useHDLVector(false);

  end  % if(inputTypesKnown)
  % -----------------------------

  % -----------------------------
   if (this_block.inputRatesKnown)
     setup_as_single_rate(this_block,'clk','ce')
   end  % if(inputRatesKnown)
  % -----------------------------

    % (!) Set the inout port rate to be the same as the first input 
    %     rate. Change the following code if this is untrue.
    uniqueInputRates = unique(this_block.getInputRates);


  % Add addtional source files as needed.
  %  |-------------
  %  | Add files in the order in which they should be compiled.
  %  | If two files "a.vhd" and "b.vhd" contain the entities
  %  | entity_a and entity_b, and entity_a contains a
  %  | component of type entity_b, the correct sequence of
  %  | addFile() calls would be:
  %  |    this_block.addFile('b.vhd');
  %  |    this_block.addFile('a.vhd');
  %  |-------------

  basroot = getenv('BASROOT');
  this_block.addFile([basroot '/sdk/fpga/NutaqIPLib/pcores/ntq_event_latch_v1_00_a/sysgen/ntq_event_latch_imp.vhd']);
  this_block.addFile([basroot '/sdk/fpga/NutaqIPLib/pcores/ntq_event_latch_v1_00_a/hdl/vhdl/ntq_event_latch.vhd']);
  
return;


% ------------------------------------------------------------

function setup_as_single_rate(block,clkname,cename) 
  inputRates = block.inputRates; 
  uniqueInputRates = unique(inputRates); 
  if (length(uniqueInputRates)==1 & uniqueInputRates(1)==Inf) 
    block.addError('The inputs to this block cannot all be constant.'); 
    return; 
  end 
  if (uniqueInputRates(end) == Inf) 
     hasConstantInput = true; 
     uniqueInputRates = uniqueInputRates(1:end-1); 
  end 
  if (length(uniqueInputRates) ~= 1) 
    block.addError('The inputs to this block must run at a single rate.'); 
    return; 
  end 
  theInputRate = uniqueInputRates(1); 
  for i = 1:block.numSimulinkOutports 
     block.outport(i).setRate(theInputRate); 
  end 
  block.addClkCEPair(clkname,cename,theInputRate); 
  return; 

% ------------------------------------------------------------

