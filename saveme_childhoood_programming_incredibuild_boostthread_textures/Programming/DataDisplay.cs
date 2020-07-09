using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace CommonSharpControls.Controls
{
	/// <summary>
	/// Summary description for DataDisplay.
	/// </summary>
	public class DataDisplay : System.Windows.Forms.UserControl
	{
		private System.Windows.Forms.DataGrid dgData;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		DataTable mTable;

		public DataDisplay()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitializeComponent call
			
			mTable = new DataTable("Data");
			mTable.Columns.Add(new DataColumn("Name",typeof(string)));
			mTable.Columns.Add(new DataColumn("Value",typeof(string)));
			mTable.Columns[0].ReadOnly = true;
			DataView ds = new DataView(mTable);
			dgData.DataSource = ds;
		}

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.dgData = new System.Windows.Forms.DataGrid();
			((System.ComponentModel.ISupportInitialize)(this.dgData)).BeginInit();
			this.SuspendLayout();
			// 
			// dgData
			// 
			this.dgData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.dgData.DataMember = "";
			this.dgData.HeaderForeColor = System.Drawing.SystemColors.ControlText;
			this.dgData.Location = new System.Drawing.Point(8, 8);
			this.dgData.Name = "dgData";
			this.dgData.PreferredColumnWidth = 250;
			this.dgData.Size = new System.Drawing.Size(264, 520);
			this.dgData.TabIndex = 0;
			// 
			// DataDisplay
			// 
			this.Controls.Add(this.dgData);
			this.Name = "DataDisplay";
			this.Size = new System.Drawing.Size(280, 536);
			((System.ComponentModel.ISupportInitialize)(this.dgData)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		public void ChangeData(string Name, string Value)
		{
			for(int i = 0; i < mTable.Rows.Count; i++) 
			{
				if(mTable.Rows[i][0].ToString() == Name)
				{
					mTable.Rows[i][1] = Value;
					return;
				}
			}

			DataRow dr = mTable.NewRow();
			dr[0] = Name;
			dr[1] = Value;
			mTable.Rows.Add(dr);
		}

		public void RemoveData(string Name)
		{
			for(int i = 0; i < mTable.Rows.Count; i++) 
			{
				if(mTable.Rows[i][0].ToString() == Name) 
				{
					mTable.Rows.Remove(mTable.Rows[i]);
					return;
				}
			}
		}
	}
}
