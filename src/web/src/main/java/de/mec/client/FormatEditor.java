/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package de.mec.client;

import de.mec.client.model.Format;
import de.mec.client.model.FormatProperties;
import com.google.gwt.core.shared.GWT;
import com.google.gwt.editor.client.Editor;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.SimplePanel;
import com.sencha.gxt.cell.core.client.form.ComboBoxCell;
import com.sencha.gxt.data.shared.ListStore;
import com.sencha.gxt.widget.core.client.form.ComboBox;
import com.sencha.gxt.widget.core.client.form.ConverterEditorAdapter;
import de.mec.client.model.tools.FormatIdConverter;
import java.util.List;

/**
 *
 * @author jhoelscher
 */
public class FormatEditor extends SimplePanel implements Editor<Format> {

  @Ignore
  ComboBox<Format> formatCombo;
  ConverterEditorAdapter<String, Format, ComboBox<Format>> id;

  public FormatEditor() {
    Grid grid = new Grid(4, 3);
    setWidget(grid);
    grid.setSize("200", "200");

    MHiveService mhiveService = new MHiveService();
    FormatProperties cprops = GWT.create(FormatProperties.class);

    final ListStore<Format> store = new ListStore<Format>(cprops.id());
    mhiveService.getFormats(new AsyncCallback<List<Format>>() {

      public void onFailure(Throwable caught) {
        throw new UnsupportedOperationException("Not supported yet.");
      }

      public void onSuccess(List<Format> result) {
        store.addAll(result);
      }
    });
    formatCombo = new ComboBox<Format>(store, cprops.longName());
    formatCombo.setTriggerAction(ComboBoxCell.TriggerAction.ALL);
    formatCombo.setForceSelection(true);
    formatCombo.setWidth(200);
    id = new ConverterEditorAdapter<String, Format, ComboBox<Format>>(formatCombo, new FormatIdConverter(store));
    grid.setWidget(0, 1, formatCombo);

  }
}
