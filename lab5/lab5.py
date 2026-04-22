import os
import glob
import pandas as pd
import streamlit as st
import plotly.express as px

st.set_page_config(page_title="NOAA Analysis", layout="wide")

st.title("Laboratory Work No. 5: NOAA Data Analysis")
st.markdown("\n**Objective:** Interactive visualization of VCI, TCI, VHI time series across regions of Ukraine.")
st.divider()

@st.cache_data
def load_data():
    path = 'data/'
    all_files = glob.glob(os.path.join(path, "*.csv"))
    
    regions_dict = {
        1: 'Vinnytsia', 2: 'Volyn', 3: 'Dnipropetrovsk', 4: 'Donetsk', 
        5: 'Zhytomyr', 6: 'Zakarpattia', 7: 'Zaporizhzhia', 8: 'Ivano-Frankivsk', 
        9: 'Kyiv', 10: 'Kirovohrad', 11: 'Luhansk', 12: 'Lviv', 
        13: 'Mykolaiv', 14: 'Odesa', 15: 'Poltava', 16: 'Rivne', 
        17: 'Sumy', 18: 'Ternopil', 19: 'Kharkiv', 20: 'Kherson', 
        21: 'Khmelnytskyi', 22: 'Cherkasy', 23: 'Chernivtsi', 24: 'Chernihiv', 
        25: 'Republic of Crimea', 26: 'Kyiv City', 27: 'Sevastopol'
    }

    df_list = []
    
    for file in all_files:
        filename = os.path.basename(file)
        try:
            region_id = int(filename.split('_')[2])
        except ValueError:
            continue 
            
        df = pd.read_csv(
            file, 
            header=1, 
            names=['Year', 'Week', 'SMN', 'SMT', 'VCI', 'TCI', 'VHI'], 
            usecols=[0, 1, 2, 3, 4, 5, 6]
        )
        
        df['Year'] = df['Year'].astype(str).str.replace('<tt><pre>', '', regex=False).str.replace('</pre></tt>', '', regex=False)
        
        df['Region'] = regions_dict.get(region_id, f"Region {region_id}")
        
        df = df.dropna()
        df = df[df['VHI'] != -1]
        
        df_list.append(df)
        
    full_df = pd.concat(df_list, ignore_index=True)
    
    full_df['Year'] = full_df['Year'].astype(int)
    full_df['Week'] = full_df['Week'].astype(int)
    full_df['VCI'] = full_df['VCI'].astype(float)
    full_df['TCI'] = full_df['TCI'].astype(float)
    full_df['VHI'] = full_df['VHI'].astype(float)
    
    return full_df

df = load_data()

defaults = {
    'metric': 'VHI',
    'region': df['Region'].unique()[0] if not df.empty else 'Kyiv City',
    'weeks': (1, 52),
    'years': (int(df['Year'].min()), int(df['Year'].max())) if not df.empty else (2000, 2024),
    'sort_asc': False,
    'sort_desc': False
}

for key, value in defaults.items():
    if key not in st.session_state:
        st.session_state[key] = value

def reset_filters():
    for key, value in defaults.items():
        st.session_state[key] = value

col1, col2 = st.columns([1, 3])

with col1:
    st.header("Settings")
    
    metric = st.selectbox("Metric", ["VCI", "TCI", "VHI"], key='metric')
    region = st.selectbox("Region", df['Region'].unique(), key='region')
    
    weeks = st.slider("Weeks Interval", 1, 52, key='weeks')
    years = st.slider("Years Interval", int(df['Year'].min()), int(df['Year'].max()), key='years')
    
    st.write("Data Sorting:")
    sort_asc = st.checkbox("Ascending", key='sort_asc')
    sort_desc = st.checkbox("Descending", key='sort_desc')
    
    st.button("Reset Filters", on_click=reset_filters, use_container_width=True)

df_period = df[(df['Year'].between(st.session_state.years[0], st.session_state.years[1])) & 
               (df['Week'].between(st.session_state.weeks[0], st.session_state.weeks[1]))]

df_filtered = df_period[df_period['Region'] == st.session_state.region]

sort_warning = False
if st.session_state.sort_asc and st.session_state.sort_desc:
    sort_warning = True
elif st.session_state.sort_asc:
    df_filtered = df_filtered.sort_values(by=st.session_state.metric, ascending=True)
elif st.session_state.sort_desc:
    df_filtered = df_filtered.sort_values(by=st.session_state.metric, ascending=False)

with col2:
    if sort_warning:
        st.warning("Both sorting types selected! Data is displayed without sorting.")
        
    tab1, tab2, tab3 = st.tabs(["Data Table", "Index Dynamics", "Regions Comparison"])
    
    with tab1:
        st.subheader(f"{st.session_state.metric} data for region: {st.session_state.region}")
        st.dataframe(df_filtered, use_container_width=True)
        
    with tab2:
        st.subheader(f"{st.session_state.metric} Dynamics ({st.session_state.years[0]}-{st.session_state.years[1]})")
        fig1 = px.line(df_filtered, x='Week', y=st.session_state.metric, color='Year', 
                       markers=True, title=f"{st.session_state.metric} changes by weeks")
        st.plotly_chart(fig1, use_container_width=True)
        
    with tab3:
        st.subheader(f"Comparison of average {st.session_state.metric} for the selected period")
        df_comp = df_period.groupby('Region')[st.session_state.metric].mean().reset_index()
        fig2 = px.bar(df_comp, x='Region', y=st.session_state.metric, 
                      title=f"Average {st.session_state.metric} across all regions")
        
        colors = ['red' if r == st.session_state.region else '#636efa' for r in df_comp['Region']]
        fig2.update_traces(marker_color=colors)
        st.plotly_chart(fig2, use_container_width=True)