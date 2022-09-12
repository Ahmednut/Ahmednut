
function lyt_mi125_record_if_imp_config(this_block)

  this_block.setTopLevelLanguage('VHDL');

  this_block.setEntityName('lyt_mi125_record_if_imp');

  % System Generator has to assume that your entity  has a combinational feed through; 
  %   if it  doesn't, then comment out the following line:
  this_block.tagAsCombinational;

  this_block.addSimulinkInport('iv14_AdcDataCh1_p');
  this_block.addSimulinkInport('iv14_AdcDataCh2_p');
  this_block.addSimulinkInport('iv14_AdcDataCh3_p');
  this_block.addSimulinkInport('iv14_AdcDataCh4_p');
  this_block.addSimulinkInport('iv14_AdcDataCh5_p');
  this_block.addSimulinkInport('iv14_AdcDataCh6_p');
  this_block.addSimulinkInport('iv14_AdcDataCh7_p');
  this_block.addSimulinkInport('iv14_AdcDataCh8_p');
  this_block.addSimulinkInport('iv14_AdcDataCh9_p');
  this_block.addSimulinkInport('iv14_AdcDataCh10_p');
  this_block.addSimulinkInport('iv14_AdcDataCh11_p');
  this_block.addSimulinkInport('iv14_AdcDataCh12_p');
  this_block.addSimulinkInport('iv14_AdcDataCh13_p');
  this_block.addSimulinkInport('iv14_AdcDataCh14_p');
  this_block.addSimulinkInport('iv14_AdcDataCh15_p');
  this_block.addSimulinkInport('iv14_AdcDataCh16_p');
  this_block.addSimulinkInport('iv14_AdcDataCh17_p');
  this_block.addSimulinkInport('iv14_AdcDataCh18_p');
  this_block.addSimulinkInport('iv14_AdcDataCh19_p');
  this_block.addSimulinkInport('iv14_AdcDataCh20_p');
  this_block.addSimulinkInport('iv14_AdcDataCh21_p');
  this_block.addSimulinkInport('iv14_AdcDataCh22_p');
  this_block.addSimulinkInport('iv14_AdcDataCh23_p');
  this_block.addSimulinkInport('iv14_AdcDataCh24_p');
  this_block.addSimulinkInport('iv14_AdcDataCh25_p');
  this_block.addSimulinkInport('iv14_AdcDataCh26_p');
  this_block.addSimulinkInport('iv14_AdcDataCh27_p');
  this_block.addSimulinkInport('iv14_AdcDataCh28_p');
  this_block.addSimulinkInport('iv14_AdcDataCh29_p');
  this_block.addSimulinkInport('iv14_AdcDataCh30_p');
  this_block.addSimulinkInport('iv14_AdcDataCh31_p');
  this_block.addSimulinkInport('iv14_AdcDataCh32_p');
  this_block.addSimulinkInport('iv14_AdcDataCh33_p');
  this_block.addSimulinkInport('iv14_AdcDataCh34_p');
  this_block.addSimulinkInport('iv14_AdcDataCh35_p');
  this_block.addSimulinkInport('iv14_AdcDataCh36_p');
  this_block.addSimulinkInport('iv14_AdcDataCh37_p');
  this_block.addSimulinkInport('iv14_AdcDataCh38_p');
  this_block.addSimulinkInport('iv14_AdcDataCh39_p');
  this_block.addSimulinkInport('iv14_AdcDataCh40_p');
  this_block.addSimulinkInport('iv14_AdcDataCh41_p');
  this_block.addSimulinkInport('iv14_AdcDataCh42_p');
  this_block.addSimulinkInport('iv14_AdcDataCh43_p');
  this_block.addSimulinkInport('iv14_AdcDataCh44_p');
  this_block.addSimulinkInport('iv14_AdcDataCh45_p');
  this_block.addSimulinkInport('iv14_AdcDataCh46_p');
  this_block.addSimulinkInport('iv14_AdcDataCh47_p');
  this_block.addSimulinkInport('iv14_AdcDataCh48_p');
  this_block.addSimulinkInport('iv14_AdcDataCh49_p');
  this_block.addSimulinkInport('iv14_AdcDataCh50_p');
  this_block.addSimulinkInport('iv14_AdcDataCh51_p');
  this_block.addSimulinkInport('iv14_AdcDataCh52_p');
  this_block.addSimulinkInport('iv14_AdcDataCh53_p');
  this_block.addSimulinkInport('iv14_AdcDataCh54_p');
  this_block.addSimulinkInport('iv14_AdcDataCh55_p');
  this_block.addSimulinkInport('iv14_AdcDataCh56_p');
  this_block.addSimulinkInport('iv14_AdcDataCh57_p');
  this_block.addSimulinkInport('iv14_AdcDataCh58_p');
  this_block.addSimulinkInport('iv14_AdcDataCh59_p');
  this_block.addSimulinkInport('iv14_AdcDataCh60_p');
  this_block.addSimulinkInport('iv14_AdcDataCh61_p');
  this_block.addSimulinkInport('iv14_AdcDataCh62_p');
  this_block.addSimulinkInport('iv14_AdcDataCh63_p');
  this_block.addSimulinkInport('iv14_AdcDataCh64_p');
  this_block.addSimulinkInport('i_AdcValid_p');
  this_block.addSimulinkInport('i_DataFormat_p');
  this_block.addSimulinkInport('i_SoftOrExternalTriggSel_p');
  this_block.addSimulinkInport('i_ExternalTrigger_p');
  this_block.addSimulinkInport('i_SoftTrigger_p');
  this_block.addSimulinkInport('iv3_MuxSel_p');

  this_block.addSimulinkOutport('ov16_RecordData1_p');
  this_block.addSimulinkOutport('ov16_RecordData2_p');
  this_block.addSimulinkOutport('ov16_RecordData3_p');
  this_block.addSimulinkOutport('ov16_RecordData4_p');
  this_block.addSimulinkOutport('ov16_RecordData5_p');
  this_block.addSimulinkOutport('ov16_RecordData6_p');
  this_block.addSimulinkOutport('ov16_RecordData7_p');
  this_block.addSimulinkOutport('ov16_RecordData8_p');
  this_block.addSimulinkOutport('ov16_RecordData9_p');
  this_block.addSimulinkOutport('ov16_RecordData10_p');
  this_block.addSimulinkOutport('ov16_RecordData11_p');
  this_block.addSimulinkOutport('ov16_RecordData12_p');
  this_block.addSimulinkOutport('ov16_RecordData13_p');
  this_block.addSimulinkOutport('ov16_RecordData14_p');
  this_block.addSimulinkOutport('ov16_RecordData15_p');
  this_block.addSimulinkOutport('ov16_RecordData16_p');
  this_block.addSimulinkOutport('o_RecordValid_p');
  this_block.addSimulinkOutport('o_RecordTrigger_p');

  ov16_RecordData1_p_port = this_block.port('ov16_RecordData1_p');
  ov16_RecordData1_p_port.setType('Fix_16_0');
  ov16_RecordData2_p_port = this_block.port('ov16_RecordData2_p');
  ov16_RecordData2_p_port.setType('Fix_16_0');
  ov16_RecordData3_p_port = this_block.port('ov16_RecordData3_p');
  ov16_RecordData3_p_port.setType('Fix_16_0');
  ov16_RecordData4_p_port = this_block.port('ov16_RecordData4_p');
  ov16_RecordData4_p_port.setType('Fix_16_0');
  ov16_RecordData5_p_port = this_block.port('ov16_RecordData5_p');
  ov16_RecordData5_p_port.setType('Fix_16_0');
  ov16_RecordData6_p_port = this_block.port('ov16_RecordData6_p');
  ov16_RecordData6_p_port.setType('Fix_16_0');
  ov16_RecordData7_p_port = this_block.port('ov16_RecordData7_p');
  ov16_RecordData7_p_port.setType('Fix_16_0');
  ov16_RecordData8_p_port = this_block.port('ov16_RecordData8_p');
  ov16_RecordData8_p_port.setType('Fix_16_0');
  ov16_RecordData9_p_port = this_block.port('ov16_RecordData9_p');
  ov16_RecordData9_p_port.setType('Fix_16_0');
  ov16_RecordData10_p_port = this_block.port('ov16_RecordData10_p');
  ov16_RecordData10_p_port.setType('Fix_16_0');
  ov16_RecordData11_p_port = this_block.port('ov16_RecordData11_p');
  ov16_RecordData11_p_port.setType('Fix_16_0');
  ov16_RecordData12_p_port = this_block.port('ov16_RecordData12_p');
  ov16_RecordData12_p_port.setType('Fix_16_0');
  ov16_RecordData13_p_port = this_block.port('ov16_RecordData13_p');
  ov16_RecordData13_p_port.setType('Fix_16_0');
  ov16_RecordData14_p_port = this_block.port('ov16_RecordData14_p');
  ov16_RecordData14_p_port.setType('Fix_16_0');
  ov16_RecordData15_p_port = this_block.port('ov16_RecordData15_p');
  ov16_RecordData15_p_port.setType('Fix_16_0');
  ov16_RecordData16_p_port = this_block.port('ov16_RecordData16_p');
  ov16_RecordData16_p_port.setType('Fix_16_0');
  o_RecordValid_p_port = this_block.port('o_RecordValid_p');
  o_RecordValid_p_port.setType('UFix_1_0');
  o_RecordValid_p_port.useHDLVector(false);
  o_RecordTrigger_p_port = this_block.port('o_RecordTrigger_p');
  o_RecordTrigger_p_port.setType('UFix_1_0');
  o_RecordTrigger_p_port.useHDLVector(false);

  % -----------------------------
  if (this_block.inputTypesKnown)
    % do input type checking, dynamic output type and generic setup in this code block.

    if (this_block.port('iv14_AdcDataCh1_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh1_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh2_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh2_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh3_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh3_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh4_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh4_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh5_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh5_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh6_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh6_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh7_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh7_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh8_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh8_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh9_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh9_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh10_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh10_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh11_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh11_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh12_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh12_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh13_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh13_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh14_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh14_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh15_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh15_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh16_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh16_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh17_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh17_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh18_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh18_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh19_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh19_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh20_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh20_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh21_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh21_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh22_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh22_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh23_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh23_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh24_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh24_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh25_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh25_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh26_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh26_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh27_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh27_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh28_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh28_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh29_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh29_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh30_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh30_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh31_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh31_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh32_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh32_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh33_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh33_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh34_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh34_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh35_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh35_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh36_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh36_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh37_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh37_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh38_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh38_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh39_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh39_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh40_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh40_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh41_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh41_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh42_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh42_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh43_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh43_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh44_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh44_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh45_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh45_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh46_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh46_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh47_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh47_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh48_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh48_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh49_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh49_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh50_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh50_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh51_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh51_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh52_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh52_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh53_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh53_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh54_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh54_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh55_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh55_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh56_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh56_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh57_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh57_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh58_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh58_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh59_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh59_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh60_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh60_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh61_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh61_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh62_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh62_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh63_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh63_p" must have width=14.');
    end

    if (this_block.port('iv14_AdcDataCh64_p').width ~= 14);
      this_block.setError('Input data type for port "iv14_AdcDataCh64_p" must have width=14.');
    end

    if (this_block.port('i_AdcValid_p').width ~= 1);
      this_block.setError('Input data type for port "i_AdcValid_p" must have width=1.');
    end

    this_block.port('i_AdcValid_p').useHDLVector(false);

    if (this_block.port('i_DataFormat_p').width ~= 1);
      this_block.setError('Input data type for port "i_DataFormat_p" must have width=1.');
    end

    this_block.port('i_DataFormat_p').useHDLVector(false);

    if (this_block.port('i_SoftOrExternalTriggSel_p').width ~= 1);
      this_block.setError('Input data type for port "i_SoftOrExternalTriggSel_p" must have width=1.');
    end

    this_block.port('i_SoftOrExternalTriggSel_p').useHDLVector(false);

    if (this_block.port('i_ExternalTrigger_p').width ~= 1);
      this_block.setError('Input data type for port "i_ExternalTrigger_p" must have width=1.');
    end

    this_block.port('i_ExternalTrigger_p').useHDLVector(false);

    if (this_block.port('i_SoftTrigger_p').width ~= 1);
      this_block.setError('Input data type for port "i_SoftTrigger_p" must have width=1.');
    end

    this_block.port('i_SoftTrigger_p').useHDLVector(false);

    if (this_block.port('iv3_MuxSel_p').width ~= 3);
      this_block.setError('Input data type for port "iv3_MuxSel_p" must have width=3.');
    end

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
  this_block.addFile([basroot '/sdk/fpga/NutaqIPLib/pcores/lyt_mi125_record_if_v1_00_a/sysgen/lyt_mi125_record_if_imp.vhd']);
  this_block.addFile([basroot '/sdk/fpga/NutaqIPLib/pcores/lyt_mi125_record_if_v1_00_a/hdl/vhdl//lyt_mi125_record_if.vhd']);

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

