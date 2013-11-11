/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.CompositeEditor;
import com.google.gwt.editor.client.EditorDelegate;
import com.google.gwt.editor.client.LeafValueEditor;
import com.google.gwt.editor.client.adapters.ListEditor;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Composite;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.container.FlowLayoutContainer;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
import de.mec.client.editor.SliderItemEditor;
import de.mec.client.editor.TextItemEditor;
import de.mec.client.model.Codec;
import de.mec.client.model.CodecProperties;
import de.mec.client.model.tools.CodecIdConverter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author jan.hoelscher
 */
public class VideoPropertyListEditor extends Composite implements
        CompositeEditor<List<PropertyItem>, PropertyItem, PropertyItemEditor<String,String>>//,
/*
 * LeafValueEditor<Map<String,String>>
 */ {

  private List<PropertyItem> _currentData;
  private EditorDelegate<List<PropertyItem>> _delegate;
  EditorChain<PropertyItem, PropertyItemEditor<String,String>> _chain;
  private static final Logger logger = Logger.getLogger(VideoEditor.class.toString());
  private List<PropertyItemEditor> editors = new ArrayList<PropertyItemEditor>();
  private FlowLayoutContainer vlc;
  private ComboBox<Codec> codecCombo;
  private ConverterEditorAdapter<String, Codec, ComboBox<Codec>> codecId;
  private  ListStore<Codec> codecs;
  public VideoPropertyListEditor() {
    vlc = new FlowLayoutContainer();
    initWidget(vlc);
    CodecProperties cprops = GWT.create(CodecProperties.class);
    MHiveService mhiveService = new MHiveService();
     codecs = new ListStore<Codec>(cprops.id());
    mhiveService.getVideoCodecs(new AsyncCallback<List<Codec>>() {

      public void onFailure(Throwable caught) {
        throw new UnsupportedOperationException("Not supported yet.");
      }

      public void onSuccess(List<Codec> result) {
        //throw new UnsupportedOperationException("Not supported yet.");
        codecs.addAll(result);
        //logger.log(Level.INFO,"CodecId:"+result.size());
      }
    });
    codecCombo = new ComboBox<Codec>(codecs, cprops.longName());
    codecCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
    codecCombo.setForceSelection(true);
    codecCombo.setWidth(400);
    vlc.add(codecCombo);

    //codecId = new ConverterEditorAdapter<String, Codec, ComboBox<Codec>>(codecCombo, new CodecIdConverter(codecs));

  }

  public PropertyItemEditor createEditorForTraversal() {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  public String getPathElement(PropertyItemEditor subEditor) {
    //logger.log(Level.INFO, "subEditor:" + subEditor);
    return "";//"[" + editors.indexOf(subEditor) + "]";
  }

  public void setEditorChain(EditorChain<PropertyItem, PropertyItemEditor<String,String>> chain) {
    _chain = chain;
  }

  public void flush() {
    for (PropertyItemEditor editor : editors) {
      PropertyItem entry = _chain.getValue(editor);
      logger.log(Level.INFO, "flush key=" + entry.getKey() + " val:" + entry.getVal());
      //_currentData.put(entry.getKey(), entry.getValue());
    }
  }

  public void onPropertyChange(String... paths) {
    throw new UnsupportedOperationException("Not supported yet.");
  }

  public void setValue(List<PropertyItem> value) {
    //logger.log(Level.INFO,"data:"+value.size());
    _currentData = value;
    //value.clear();
    value.add(new PropertyItem("left","right"));
    for (PropertyItem entry : value) {
      PropertyItemEditor editor = new TextItemEditor();
      editor.setLabel(entry.getKey());
      if(entry.getKey().equals("id")){
        //codecId.setEditorChain(_chain);
        //codecId.setValue(entry.getVal());
        codecCombo.setValue(new CodecIdConverter(codecs).convertModelValue(entry.getVal()));
        continue;
      }
      //PropertyItem item=new PropertyItem(entry.getKey(), entry.getValue());
      editors.add(editor);
      _chain.attach(entry, editor);
      vlc.add(editor);

    }
  }

  public void setDelegate(EditorDelegate<List<PropertyItem>> delegate) {
    _delegate = delegate;
  }

  public List<PropertyItem> getValue() {
    return _currentData;
  }
}
