using Patchwork.Properties;
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;

namespace Patchwork
{
    public interface ISettingsLocker
    {
        MruList LoadMruList();
        void SaveMruList(MruList data);

        string LoadCurrentArrangementFilename();
        void SaveCurrentArrangementFilename(string currentFileName);

        int LoadScale();
        void SaveScale(int value);

        bool LoadGridVisibility();
        void SaveGridVisibility(bool value);

        List<string> LoadCurrentTilesetFilenames();
        int LoadCurrentTilesetTileSize();
        void SaveCurrentTilesets(IEnumerable<string> filenames, int tileSize);
    }

    class SettingsLocker : ISettingsLocker
    {
        Settings backingStore;

        public SettingsLocker(Settings backingStore)
        {
            this.backingStore = backingStore;
        }

        public MruList LoadMruList()
        {
            return new MruList(backingStore.MruList.Split(
                new[] { '|' }, StringSplitOptions.RemoveEmptyEntries));
        }

        public void SaveMruList(MruList data)
        {
            var text = new StringBuilder();

            for (int i = 0; i < data.Count; i++)
            {
                if (0 != i)
                    text.Append('|');

                text.Append(data[i]);
            }

            backingStore.MruList = text.ToString();
        }

        public void Persist()
        {
            backingStore.Save();
        }

        public string LoadCurrentArrangementFilename()
        {
            return backingStore.LatestArrangementFilename;
        }

        public void SaveCurrentArrangementFilename(string filename)
        {
            backingStore.LatestArrangementFilename = filename;
        }

        public int LoadScale()
        {
            return backingStore.Scale;
        }

        public void SaveScale(int value)
        {
            backingStore.Scale = value;
        }

        public bool LoadGridVisibility()
        {
            return backingStore.GridVisible;
        }

        public void SaveGridVisibility(bool value)
        {
            backingStore.GridVisible = value;
        }

        public List<string> LoadCurrentTilesetFilenames()
        {
            if (backingStore.LatestTilesetFilenames == null)
                return new List<string>();

            return backingStore.LatestTilesetFilenames.Cast<string>().ToList();
        }

        public int LoadCurrentTilesetTileSize()
        {
            return backingStore.LatestTilesetTileSize;
        }

        public void SaveCurrentTilesets(IEnumerable<string> filenames, int tileSize)
        {
            backingStore.LatestTilesetFilenames = new StringCollection();
            backingStore.LatestTilesetFilenames.AddRange(filenames.ToArray());
            backingStore.LatestTilesetTileSize = tileSize;
        }
    }
}
